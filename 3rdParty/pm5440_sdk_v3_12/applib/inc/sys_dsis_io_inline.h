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
 *     and register accessor functions for the sys_dsis block
 *****************************************************************************/
#ifndef _SYS_DSIS_IO_INLINE_H
#define _SYS_DSIS_IO_INLINE_H

#include "sifd_api.h"
#include "sys_dsis_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SYS_DSIS_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for sys_dsis
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
    sifd_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} sys_dsis_buffer_t;
static INLINE void sys_dsis_buffer_init( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void sys_dsis_buffer_init( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "sys_dsis_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void sys_dsis_buffer_flush( sys_dsis_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sys_dsis_buffer_flush( sys_dsis_buffer_t *b_ptr )
{
    IOLOG( "sys_dsis_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 sys_dsis_reg_read( sys_dsis_buffer_t *b_ptr,
                                        sifd_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_read( sys_dsis_buffer_t *b_ptr,
                                        sifd_handle_t *h_ptr,
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
static INLINE void sys_dsis_reg_write( sys_dsis_buffer_t *b_ptr,
                                       sifd_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_write( sys_dsis_buffer_t *b_ptr,
                                       sifd_handle_t *h_ptr,
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

static INLINE void sys_dsis_field_set( sys_dsis_buffer_t *b_ptr,
                                       sifd_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_set( sys_dsis_buffer_t *b_ptr,
                                       sifd_handle_t *h_ptr,
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

static INLINE void sys_dsis_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
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

static INLINE void sys_dsis_burst_read( sys_dsis_buffer_t *b_ptr,
                                        sifd_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void sys_dsis_burst_read( sys_dsis_buffer_t *b_ptr,
                                        sifd_handle_t *h_ptr,
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

static INLINE void sys_dsis_burst_write( sys_dsis_buffer_t *b_ptr,
                                         sifd_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void sys_dsis_burst_write( sys_dsis_buffer_t *b_ptr,
                                         sifd_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE sys_dsis_poll( sys_dsis_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_poll( sys_dsis_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
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
 *  register access functions for sys_dsis
 * ==================================================================================
 */

static INLINE void sys_dsis_reg_ENET_TX_CLK_SELECT_write( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_ENET_TX_CLK_SELECT_write( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_ENET_TX_CLK_SELECT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_ENET_TX_CLK_SELECT,
                        value);
}

static INLINE void sys_dsis_reg_ENET_TX_CLK_SELECT_field_set( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_ENET_TX_CLK_SELECT_field_set( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_ENET_TX_CLK_SELECT_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_ENET_TX_CLK_SELECT,
                        mask,
                        PMC_DSIS_REG_ENET_TX_CLK_SELECT_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_ENET_TX_CLK_SELECT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_ENET_TX_CLK_SELECT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_ENET_TX_CLK_SELECT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_ENET_TX_CLK_SELECT_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_SERDES_TX_LANE_EN_write( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_SERDES_TX_LANE_EN_write( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_SERDES_TX_LANE_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_SERDES_TX_LANE_EN,
                        value);
}

static INLINE void sys_dsis_reg_SERDES_TX_LANE_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_SERDES_TX_LANE_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_SERDES_TX_LANE_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_SERDES_TX_LANE_EN,
                        mask,
                        PMC_DSIS_REG_SERDES_TX_LANE_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_SERDES_TX_LANE_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_SERDES_TX_LANE_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_SERDES_TX_LANE_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_SERDES_TX_LANE_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_RESET_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_RESET_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_RESET_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_RESET,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_RESET_field_set( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_RESET_field_set( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_RESET_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_RESET,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_RESET_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_RESET_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_RESET_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_TX_FIFO_RESET);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_RESET_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_MODE_SELECT_write( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_MODE_SELECT_write( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_MODE_SELECT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_MODE_SELECT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_MODE_SELECT_field_set( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_MODE_SELECT_field_set( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_MODE_SELECT_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_MODE_SELECT,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_MODE_SELECT_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_MODE_SELECT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_MODE_SELECT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_TX_FIFO_MODE_SELECT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_MODE_SELECT_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_CENTERING_MODE_write( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_CENTERING_MODE_write( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_CENTERING_MODE_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_CENTERING_MODE,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_CENTERING_MODE_field_set( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_CENTERING_MODE_field_set( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_CENTERING_MODE_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_CENTERING_MODE,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_CENTERING_MODE_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_CENTERING_MODE_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_CENTERING_MODE_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_TX_FIFO_CENTERING_MODE);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_CENTERING_MODE_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_write( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_write( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_CENTERING_REQUEST,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_field_set( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_field_set( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_CENTERING_REQUEST,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_CENTERING_REQUEST_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_TX_FIFO_CENTERING_REQUEST);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                                 sifd_handle_t *h_ptr,
                                                                                 UINT32  N,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                                 sifd_handle_t *h_ptr,
                                                                                 UINT32  N,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11(N),
                        value);
}

static INLINE void sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                     sifd_handle_t *h_ptr,
                                                                                     UINT32  N,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                     sifd_handle_t *h_ptr,
                                                                                     UINT32  N,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_field_set", N, mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11(N),
                        mask,
                        PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                                  sifd_handle_t *h_ptr,
                                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                                  sifd_handle_t *h_ptr,
                                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32  N,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32  N,
                                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11(N),
                        value);
}

static INLINE void sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                   sifd_handle_t *h_ptr,
                                                                                   UINT32  N,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                   sifd_handle_t *h_ptr,
                                                                                   UINT32  N,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_field_set", N, mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11(N),
                        mask,
                        PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
                                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                                sifd_handle_t *h_ptr,
                                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11(N),
                        value);
}

static INLINE void sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                  sifd_handle_t *h_ptr,
                                                                                  UINT32  N,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                  sifd_handle_t *h_ptr,
                                                                                  UINT32  N,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_field_set", N, mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11(N),
                        mask,
                        PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11(N),
                        value);
}

static INLINE void sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                  sifd_handle_t *h_ptr,
                                                                                  UINT32  N,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                  sifd_handle_t *h_ptr,
                                                                                  UINT32  N,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_field_set", N, mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11(N),
                        mask,
                        PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11(N),
                        value);
}

static INLINE void sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                  sifd_handle_t *h_ptr,
                                                                                  UINT32  N,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                  sifd_handle_t *h_ptr,
                                                                                  UINT32  N,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_field_set", N, mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11(N),
                        mask,
                        PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                               sifd_handle_t *h_ptr,
                                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_write( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11(N),
                        value);
}

static INLINE void sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_field_set", N, mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11(N),
                        mask,
                        PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_read( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_write( sys_dsis_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_write( sys_dsis_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT(N),
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_field_set", N, mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT(N),
                        mask,
                        PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_read( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_read( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_write( sys_dsis_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_write( sys_dsis_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO(N),
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                 sifd_handle_t *h_ptr,
                                                                                 UINT32  N,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_field_set( sys_dsis_buffer_t *b_ptr,
                                                                                 sifd_handle_t *h_ptr,
                                                                                 UINT32  N,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_field_set", N, mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO(N),
                        mask,
                        PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_read( sys_dsis_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_read( sys_dsis_buffer_t *b_ptr,
                                                                              sifd_handle_t *h_ptr,
                                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_0_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_0_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_0_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_0_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_0_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_0_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_0_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_0_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_0_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_0_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_0_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_0_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_0_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_1_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_1_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_1_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_1_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_1_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_1_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_1_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_1_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_1_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_1_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_1_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_1_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_1_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_2_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_2_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_2_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_2_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_2_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_2_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_2_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_2_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_2_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_2_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_2_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_2_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_2_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_3_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_3_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_3_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_3_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_3_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_3_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_3_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_3_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_3_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_3_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_3_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_3_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_3_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_4_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_4_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_4_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_4_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_4_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_4_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_4_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_4_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_4_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_4_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_4_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_4_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_4_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_5_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_5_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_5_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_5_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_5_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_5_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_5_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_5_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_5_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_5_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_5_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_5_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_5_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_6_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_6_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_6_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_6_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_6_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_6_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_6_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_6_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_6_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_6_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_6_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_6_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_6_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_7_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_7_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_7_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_7_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_7_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_7_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_7_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_7_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_7_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_7_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_7_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_7_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_7_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_8_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_8_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_8_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_8_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_8_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_8_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_8_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_8_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_8_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_8_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_8_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_8_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_8_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_9_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_9_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_9_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_9_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_9_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_9_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_9_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_9_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_9_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_9_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_9_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_9_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_9_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_10_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_10_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_10_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_10_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_10_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_10_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_10_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_10_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_10_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_10_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_10_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_10_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_10_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sys_dsis_reg_TX_FIFO_11_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_11_INT_EN_write( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_11_INT_EN_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_11_INT_EN,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_11_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_11_INT_EN_field_set( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_11_INT_EN_field_set", mask, ofs, value );
    sys_dsis_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_11_INT_EN,
                        mask,
                        PMC_DSIS_REG_TX_FIFO_11_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_11_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_11_INT_EN_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_11_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_11_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE UINT32 sys_dsis_reg_INT_SUMMARY_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_INT_SUMMARY_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_INT_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_INT_SUMMARY_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_INT_SUMMARY_poll( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_INT_SUMMARY_poll( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_INT_SUMMARY_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_INT_SUMMARY,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_0_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_0_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_0_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_0_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_0_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_0_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_0_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_0_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_0_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_0_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_0_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_0_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_0_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_0_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_0_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_0_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_1_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_1_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_1_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_1_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_1_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_1_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_1_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_1_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_1_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_1_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_1_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_1_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_1_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_1_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_1_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_1_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_2_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_2_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_2_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_2_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_2_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_2_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_2_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_2_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_2_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_2_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_2_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_2_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_2_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_2_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_2_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_2_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_3_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_3_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_3_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_3_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_3_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_3_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_3_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_3_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_3_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_3_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_3_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_3_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_3_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_3_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_3_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_3_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_4_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_4_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_4_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_4_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_4_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_4_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_4_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_4_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_4_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_4_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_4_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_4_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_4_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_4_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_4_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_4_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_5_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_5_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_5_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_5_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_5_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_5_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_5_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_5_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_5_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_5_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_5_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_5_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_5_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_5_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_5_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_5_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_6_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_6_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_6_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_6_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_6_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_6_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_6_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_6_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_6_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_6_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_6_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_6_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_6_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_6_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_6_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_6_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_7_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_7_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_7_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_7_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_7_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_7_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_7_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_7_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_7_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_7_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_7_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_7_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_7_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_7_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_7_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_7_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_8_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_8_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_8_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_8_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_8_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_8_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_8_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_8_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_8_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_8_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_8_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_8_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_8_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_8_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_8_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_8_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_9_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_9_INT_write( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_9_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_9_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_9_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_9_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_9_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_9_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_9_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_9_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_9_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_9_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_9_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_9_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_9_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_9_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_10_INT_write( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_10_INT_write( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_10_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_10_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_10_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_10_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_10_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_10_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_10_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_10_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_10_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_10_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_10_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_10_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_10_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_10_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void sys_dsis_reg_TX_FIFO_11_INT_write( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_11_INT_write( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sys_dsis_reg_TX_FIFO_11_INT_write", value );
    sys_dsis_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DSIS_REG_TX_FIFO_11_INT,
                        value);
}

static INLINE void sys_dsis_reg_TX_FIFO_11_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_reg_TX_FIFO_11_INT_action_on_write_field_set( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sys_dsis_reg_TX_FIFO_11_INT_action_on_write_field_set", mask, ofs, value );
    sys_dsis_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_DSIS_REG_TX_FIFO_11_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 sys_dsis_reg_TX_FIFO_11_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_reg_TX_FIFO_11_INT_read( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sys_dsis_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_DSIS_REG_TX_FIFO_11_INT);

    IOLOG( "%s -> 0x%08x;", "sys_dsis_reg_TX_FIFO_11_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_11_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_11_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sys_dsis_reg_TX_FIFO_11_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sys_dsis_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_DSIS_REG_TX_FIFO_11_INT,
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
static INLINE void sys_dsis_field_ENET_TX_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_ENET_TX_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_ENET_TX_CLK_SEL_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_ENET_TX_CLK_SEL_set", value );

    /* (0x000010c0 bits 11:0) field ENET_TX_CLK_SEL of register PMC_DSIS_REG_ENET_TX_CLK_SELECT */
    sys_dsis_reg_ENET_TX_CLK_SELECT_field_set( b_ptr,
                                               h_ptr,
                                               DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_MSK,
                                               DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_OFF,
                                               value);
}

static INLINE UINT32 sys_dsis_field_ENET_TX_CLK_SEL_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_ENET_TX_CLK_SEL_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000010c0 bits 11:0) field ENET_TX_CLK_SEL of register PMC_DSIS_REG_ENET_TX_CLK_SELECT */
    reg_value = sys_dsis_reg_ENET_TX_CLK_SELECT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_MSK) >> DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_ENET_TX_CLK_SEL_get", value );

    return value;
}
static INLINE void sys_dsis_field_range_ENET_TX_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_ENET_TX_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_ENET_TX_CLK_SEL_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_ENET_TX_CLK_SEL_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_ENET_TX_CLK_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000010c0 bits 11:0) field ENET_TX_CLK_SEL of register PMC_DSIS_REG_ENET_TX_CLK_SELECT */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000010c0 bits 11:0) field ENET_TX_CLK_SEL of register PMC_DSIS_REG_ENET_TX_CLK_SELECT */
        sys_dsis_reg_ENET_TX_CLK_SELECT_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_OFF + subfield_offset),
                                                   DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_ENET_TX_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_ENET_TX_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_ENET_TX_CLK_SEL_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_ENET_TX_CLK_SEL_get", stop_bit, 11 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 11) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 11;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000010c0 bits 11:0) field ENET_TX_CLK_SEL of register PMC_DSIS_REG_ENET_TX_CLK_SELECT */
    reg_value = sys_dsis_reg_ENET_TX_CLK_SELECT_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_MSK)
                  >> DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_MSK, DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_ENET_TX_CLK_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_LANE_ENABLE_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_LANE_ENABLE_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_LANE_ENABLE_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_TX_LANE_ENABLE_set", value );

    /* (0x00001144 bits 19:0) field TX_LANE_ENABLE of register PMC_DSIS_REG_SERDES_TX_LANE_EN */
    sys_dsis_reg_SERDES_TX_LANE_EN_field_set( b_ptr,
                                              h_ptr,
                                              DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_MSK,
                                              DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_OFF,
                                              value);
}

static INLINE UINT32 sys_dsis_field_TX_LANE_ENABLE_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_LANE_ENABLE_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001144 bits 19:0) field TX_LANE_ENABLE of register PMC_DSIS_REG_SERDES_TX_LANE_EN */
    reg_value = sys_dsis_reg_SERDES_TX_LANE_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_MSK) >> DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_TX_LANE_ENABLE_get", value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_LANE_ENABLE_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_LANE_ENABLE_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_LANE_ENABLE_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_LANE_ENABLE_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_LANE_ENABLE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001144 bits 19:0) field TX_LANE_ENABLE of register PMC_DSIS_REG_SERDES_TX_LANE_EN */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001144 bits 19:0) field TX_LANE_ENABLE of register PMC_DSIS_REG_SERDES_TX_LANE_EN */
        sys_dsis_reg_SERDES_TX_LANE_EN_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_OFF + subfield_offset),
                                                  DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_LANE_ENABLE_get( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_LANE_ENABLE_get( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_LANE_ENABLE_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_LANE_ENABLE_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001144 bits 19:0) field TX_LANE_ENABLE of register PMC_DSIS_REG_SERDES_TX_LANE_EN */
    reg_value = sys_dsis_reg_SERDES_TX_LANE_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_MSK)
                  >> DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_MSK, DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_LANE_ENABLE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_FIFO_RESET_set( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_FIFO_RESET_set( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_FIFO_RESET_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_TX_FIFO_RESET_set", value );

    /* (0x00001198 bits 19:0) field TX_FIFO_RESET of register PMC_DSIS_REG_TX_FIFO_RESET */
    sys_dsis_reg_TX_FIFO_RESET_field_set( b_ptr,
                                          h_ptr,
                                          DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_MSK,
                                          DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_OFF,
                                          value);
}

static INLINE UINT32 sys_dsis_field_TX_FIFO_RESET_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_FIFO_RESET_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001198 bits 19:0) field TX_FIFO_RESET of register PMC_DSIS_REG_TX_FIFO_RESET */
    reg_value = sys_dsis_reg_TX_FIFO_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_MSK) >> DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_TX_FIFO_RESET_get", value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_FIFO_RESET_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_FIFO_RESET_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_RESET_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_RESET_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001198 bits 19:0) field TX_FIFO_RESET of register PMC_DSIS_REG_TX_FIFO_RESET */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001198 bits 19:0) field TX_FIFO_RESET of register PMC_DSIS_REG_TX_FIFO_RESET */
        sys_dsis_reg_TX_FIFO_RESET_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_OFF + subfield_offset),
                                              DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_FIFO_RESET_get( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_FIFO_RESET_get( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_RESET_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_RESET_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001198 bits 19:0) field TX_FIFO_RESET of register PMC_DSIS_REG_TX_FIFO_RESET */
    reg_value = sys_dsis_reg_TX_FIFO_RESET_read(  b_ptr, h_ptr);
    field_value = (reg_value & DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_MSK)
                  >> DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_MSK, DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_FIFO_MODE_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_FIFO_MODE_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_FIFO_MODE_SEL_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_TX_FIFO_MODE_SEL_set", value );

    /* (0x0000119c bits 19:0) field TX_FIFO_MODE_SEL of register PMC_DSIS_REG_TX_FIFO_MODE_SELECT */
    sys_dsis_reg_TX_FIFO_MODE_SELECT_field_set( b_ptr,
                                                h_ptr,
                                                DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_MSK,
                                                DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_OFF,
                                                value);
}

static INLINE UINT32 sys_dsis_field_TX_FIFO_MODE_SEL_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_FIFO_MODE_SEL_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000119c bits 19:0) field TX_FIFO_MODE_SEL of register PMC_DSIS_REG_TX_FIFO_MODE_SELECT */
    reg_value = sys_dsis_reg_TX_FIFO_MODE_SELECT_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_MSK) >> DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_TX_FIFO_MODE_SEL_get", value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_FIFO_MODE_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_FIFO_MODE_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_MODE_SEL_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_MODE_SEL_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_MODE_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000119c bits 19:0) field TX_FIFO_MODE_SEL of register PMC_DSIS_REG_TX_FIFO_MODE_SELECT */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000119c bits 19:0) field TX_FIFO_MODE_SEL of register PMC_DSIS_REG_TX_FIFO_MODE_SELECT */
        sys_dsis_reg_TX_FIFO_MODE_SELECT_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_OFF + subfield_offset),
                                                    DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_FIFO_MODE_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_FIFO_MODE_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_MODE_SEL_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_MODE_SEL_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000119c bits 19:0) field TX_FIFO_MODE_SEL of register PMC_DSIS_REG_TX_FIFO_MODE_SELECT */
    reg_value = sys_dsis_reg_TX_FIFO_MODE_SELECT_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_MSK)
                  >> DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_MSK, DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_MODE_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_FIFO_AUTO_CENTER_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_FIFO_AUTO_CENTER_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_FIFO_AUTO_CENTER_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_TX_FIFO_AUTO_CENTER_set", value );

    /* (0x000011a0 bits 19:0) field TX_FIFO_AUTO_CENTER of register PMC_DSIS_REG_TX_FIFO_CENTERING_MODE */
    sys_dsis_reg_TX_FIFO_CENTERING_MODE_field_set( b_ptr,
                                                   h_ptr,
                                                   DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_MSK,
                                                   DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_OFF,
                                                   value);
}

static INLINE UINT32 sys_dsis_field_TX_FIFO_AUTO_CENTER_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_FIFO_AUTO_CENTER_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011a0 bits 19:0) field TX_FIFO_AUTO_CENTER of register PMC_DSIS_REG_TX_FIFO_CENTERING_MODE */
    reg_value = sys_dsis_reg_TX_FIFO_CENTERING_MODE_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_MSK) >> DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_TX_FIFO_AUTO_CENTER_get", value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_FIFO_AUTO_CENTER_set( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_FIFO_AUTO_CENTER_set( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_AUTO_CENTER_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_AUTO_CENTER_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_AUTO_CENTER_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000011a0 bits 19:0) field TX_FIFO_AUTO_CENTER of register PMC_DSIS_REG_TX_FIFO_CENTERING_MODE */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000011a0 bits 19:0) field TX_FIFO_AUTO_CENTER of register PMC_DSIS_REG_TX_FIFO_CENTERING_MODE */
        sys_dsis_reg_TX_FIFO_CENTERING_MODE_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_OFF + subfield_offset),
                                                       DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_FIFO_AUTO_CENTER_get( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_FIFO_AUTO_CENTER_get( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_AUTO_CENTER_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_AUTO_CENTER_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000011a0 bits 19:0) field TX_FIFO_AUTO_CENTER of register PMC_DSIS_REG_TX_FIFO_CENTERING_MODE */
    reg_value = sys_dsis_reg_TX_FIFO_CENTERING_MODE_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_MSK)
                  >> DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_MSK, DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_AUTO_CENTER_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_FIFO_CENTER_REQUEST_set( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_FIFO_CENTER_REQUEST_set( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_FIFO_CENTER_REQUEST_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_TX_FIFO_CENTER_REQUEST_set", value );

    /* (0x000011a4 bits 19:0) field TX_FIFO_CENTER_REQUEST of register PMC_DSIS_REG_TX_FIFO_CENTERING_REQUEST */
    sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_field_set( b_ptr,
                                                      h_ptr,
                                                      DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_MSK,
                                                      DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_OFF,
                                                      value);
}

static INLINE UINT32 sys_dsis_field_TX_FIFO_CENTER_REQUEST_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_FIFO_CENTER_REQUEST_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011a4 bits 19:0) field TX_FIFO_CENTER_REQUEST of register PMC_DSIS_REG_TX_FIFO_CENTERING_REQUEST */
    reg_value = sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_MSK) >> DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_TX_FIFO_CENTER_REQUEST_get", value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_set( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_set( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000011a4 bits 19:0) field TX_FIFO_CENTER_REQUEST of register PMC_DSIS_REG_TX_FIFO_CENTERING_REQUEST */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000011a4 bits 19:0) field TX_FIFO_CENTER_REQUEST of register PMC_DSIS_REG_TX_FIFO_CENTERING_REQUEST */
        sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_OFF + subfield_offset),
                                                          DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_get( sys_dsis_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_get( sys_dsis_buffer_t *b_ptr,
                                                                      sifd_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000011a4 bits 19:0) field TX_FIFO_CENTER_REQUEST of register PMC_DSIS_REG_TX_FIFO_CENTERING_REQUEST */
    reg_value = sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_MSK)
                  >> DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_MSK, DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void sys_dsis_field_RX_EN_set( sys_dsis_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RX_EN_set( sys_dsis_buffer_t *b_ptr,
                                             sifd_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_RX_EN_set", N, 11);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RX_EN_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_RX_EN_set", N, value );

    /* ((0x00001000 + (N) * 0x4) bits 10:8) field RX_EN of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
    sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_field_set( b_ptr,
                                                                      h_ptr,
                                                                      N,
                                                                      DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_MSK,
                                                                      DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_OFF,
                                                                      value);
}

static INLINE UINT32 sys_dsis_field_RX_EN_get( sys_dsis_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RX_EN_get( sys_dsis_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_RX_EN_get", N, 11);
    /* ((0x00001000 + (N) * 0x4) bits 10:8) field RX_EN of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_read( b_ptr,
                                                                             h_ptr,
                                                                             N);
    value = (reg_value & DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_MSK) >> DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_RX_EN_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_RX_EN_set( sys_dsis_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_RX_EN_set( sys_dsis_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_RX_EN_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_RX_EN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_RX_EN_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_RX_EN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001000 + (N) * 0x4) bits 10:8) field RX_EN of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
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
        /* ((0x00001000 + (N) * 0x4) bits 10:8) field RX_EN of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
        sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_field_set( b_ptr,
                                                                          h_ptr,
                                                                          N,
                                                                          subfield_mask << (DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_OFF + subfield_offset),
                                                                          DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_OFF + subfield_offset,
                                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_RX_EN_get( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_RX_EN_get( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_RX_EN_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_RX_EN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_RX_EN_get", stop_bit, 2 );
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
    /* ((0x00001000 + (N) * 0x4) bits 10:8) field RX_EN of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_read( b_ptr,
                                                                             h_ptr,
                                                                             N);
    field_value = (reg_value & DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_MSK)
                  >> DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_MSK, DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_RX_EN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_RX_LSEL_set( sys_dsis_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RX_LSEL_set( sys_dsis_buffer_t *b_ptr,
                                               sifd_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_RX_LSEL_set", N, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RX_LSEL_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_RX_LSEL_set", N, value );

    /* ((0x00001000 + (N) * 0x4) bits 7:4) field RX_LSEL of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
    sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_field_set( b_ptr,
                                                                      h_ptr,
                                                                      N,
                                                                      DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_MSK,
                                                                      DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_OFF,
                                                                      value);
}

static INLINE UINT32 sys_dsis_field_RX_LSEL_get( sys_dsis_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RX_LSEL_get( sys_dsis_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_RX_LSEL_get", N, 11);
    /* ((0x00001000 + (N) * 0x4) bits 7:4) field RX_LSEL of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_read( b_ptr,
                                                                             h_ptr,
                                                                             N);
    value = (reg_value & DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_MSK) >> DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_RX_LSEL_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_RX_LSEL_set( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_RX_LSEL_set( sys_dsis_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_RX_LSEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_RX_LSEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_RX_LSEL_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_RX_LSEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001000 + (N) * 0x4) bits 7:4) field RX_LSEL of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
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
        /* ((0x00001000 + (N) * 0x4) bits 7:4) field RX_LSEL of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
        sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_field_set( b_ptr,
                                                                          h_ptr,
                                                                          N,
                                                                          subfield_mask << (DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_OFF + subfield_offset),
                                                                          DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_OFF + subfield_offset,
                                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_RX_LSEL_get( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_RX_LSEL_get( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_RX_LSEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_RX_LSEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_RX_LSEL_get", stop_bit, 3 );
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
    /* ((0x00001000 + (N) * 0x4) bits 7:4) field RX_LSEL of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_read( b_ptr,
                                                                             h_ptr,
                                                                             N);
    field_value = (reg_value & DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_MSK)
                  >> DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_MSK, DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_RX_LSEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_RX_SEL_set( sys_dsis_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RX_SEL_set( sys_dsis_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_RX_SEL_set", N, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RX_SEL_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_RX_SEL_set", N, value );

    /* ((0x00001000 + (N) * 0x4) bits 3:0) field RX_SEL of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
    sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_field_set( b_ptr,
                                                                      h_ptr,
                                                                      N,
                                                                      DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_MSK,
                                                                      DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_OFF,
                                                                      value);
}

static INLINE UINT32 sys_dsis_field_RX_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RX_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_RX_SEL_get", N, 11);
    /* ((0x00001000 + (N) * 0x4) bits 3:0) field RX_SEL of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_read( b_ptr,
                                                                             h_ptr,
                                                                             N);
    value = (reg_value & DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_MSK) >> DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_RX_SEL_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_RX_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_RX_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_RX_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_RX_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_RX_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_RX_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001000 + (N) * 0x4) bits 3:0) field RX_SEL of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
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
        /* ((0x00001000 + (N) * 0x4) bits 3:0) field RX_SEL of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
        sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_field_set( b_ptr,
                                                                          h_ptr,
                                                                          N,
                                                                          subfield_mask << (DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_OFF + subfield_offset),
                                                                          DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_OFF + subfield_offset,
                                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_RX_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_RX_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_RX_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_RX_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_RX_SEL_get", stop_bit, 3 );
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
    /* ((0x00001000 + (N) * 0x4) bits 3:0) field RX_SEL of register PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_read( b_ptr,
                                                                             h_ptr,
                                                                             N);
    field_value = (reg_value & DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_MSK)
                  >> DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_MSK, DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_RX_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_RX_I_LSEL_set( sys_dsis_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RX_I_LSEL_set( sys_dsis_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_RX_I_LSEL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RX_I_LSEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_RX_I_LSEL_set", N, value );

    /* ((0x00001030 + (N) * 0x4) bits 5:4) field RX_I_LSEL of register PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 index N=0..11 */
    sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                    h_ptr,
                                                                    N,
                                                                    DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_MSK,
                                                                    DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_OFF,
                                                                    value);
}

static INLINE UINT32 sys_dsis_field_RX_I_LSEL_get( sys_dsis_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RX_I_LSEL_get( sys_dsis_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_RX_I_LSEL_get", N, 11);
    /* ((0x00001030 + (N) * 0x4) bits 5:4) field RX_I_LSEL of register PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_read( b_ptr,
                                                                           h_ptr,
                                                                           N);
    value = (reg_value & DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_MSK) >> DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_RX_I_LSEL_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_RX_I_LSEL_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_RX_I_LSEL_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_RX_I_LSEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_RX_I_LSEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_RX_I_LSEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_RX_I_LSEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001030 + (N) * 0x4) bits 5:4) field RX_I_LSEL of register PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 index N=0..11 */
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
        /* ((0x00001030 + (N) * 0x4) bits 5:4) field RX_I_LSEL of register PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 index N=0..11 */
        sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                        h_ptr,
                                                                        N,
                                                                        subfield_mask << (DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_OFF + subfield_offset),
                                                                        DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_RX_I_LSEL_get( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_RX_I_LSEL_get( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_RX_I_LSEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_RX_I_LSEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_RX_I_LSEL_get", stop_bit, 1 );
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
    /* ((0x00001030 + (N) * 0x4) bits 5:4) field RX_I_LSEL of register PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_read( b_ptr,
                                                                           h_ptr,
                                                                           N);
    field_value = (reg_value & DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_MSK)
                  >> DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_MSK, DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_RX_I_LSEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_RX_I_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RX_I_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_RX_I_SEL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RX_I_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_RX_I_SEL_set", N, value );

    /* ((0x00001030 + (N) * 0x4) bits 1:0) field RX_I_SEL of register PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 index N=0..11 */
    sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                    h_ptr,
                                                                    N,
                                                                    DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_MSK,
                                                                    DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_OFF,
                                                                    value);
}

static INLINE UINT32 sys_dsis_field_RX_I_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RX_I_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_RX_I_SEL_get", N, 11);
    /* ((0x00001030 + (N) * 0x4) bits 1:0) field RX_I_SEL of register PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_read( b_ptr,
                                                                           h_ptr,
                                                                           N);
    value = (reg_value & DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_MSK) >> DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_RX_I_SEL_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_RX_I_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_RX_I_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_RX_I_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_RX_I_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_RX_I_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_RX_I_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001030 + (N) * 0x4) bits 1:0) field RX_I_SEL of register PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 index N=0..11 */
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
        /* ((0x00001030 + (N) * 0x4) bits 1:0) field RX_I_SEL of register PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 index N=0..11 */
        sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                        h_ptr,
                                                                        N,
                                                                        subfield_mask << (DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_OFF + subfield_offset),
                                                                        DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_RX_I_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_RX_I_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_RX_I_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_RX_I_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_RX_I_SEL_get", stop_bit, 1 );
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
    /* ((0x00001030 + (N) * 0x4) bits 1:0) field RX_I_SEL of register PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_read( b_ptr,
                                                                           h_ptr,
                                                                           N);
    field_value = (reg_value & DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_MSK)
                  >> DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_MSK, DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_RX_I_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_CLK_EN_set( sys_dsis_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_CLK_EN_set( sys_dsis_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_CLK_EN_set", N, 11);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_CLK_EN_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_TX_CLK_EN_set", N, value );

    /* ((0x00001060 + (N) * 0x4) bits 6:4) field TX_CLK_EN of register PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11 index N=0..11 */
    sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                   h_ptr,
                                                                   N,
                                                                   DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_MSK,
                                                                   DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_OFF,
                                                                   value);
}

static INLINE UINT32 sys_dsis_field_TX_CLK_EN_get( sys_dsis_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_CLK_EN_get( sys_dsis_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_CLK_EN_get", N, 11);
    /* ((0x00001060 + (N) * 0x4) bits 6:4) field TX_CLK_EN of register PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    value = (reg_value & DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_MSK) >> DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_TX_CLK_EN_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_CLK_EN_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_CLK_EN_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_CLK_EN_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_CLK_EN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_CLK_EN_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_CLK_EN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001060 + (N) * 0x4) bits 6:4) field TX_CLK_EN of register PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11 index N=0..11 */
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
        /* ((0x00001060 + (N) * 0x4) bits 6:4) field TX_CLK_EN of register PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11 index N=0..11 */
        sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                       h_ptr,
                                                                       N,
                                                                       subfield_mask << (DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_OFF + subfield_offset),
                                                                       DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_CLK_EN_get( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_CLK_EN_get( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_CLK_EN_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_CLK_EN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_CLK_EN_get", stop_bit, 2 );
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
    /* ((0x00001060 + (N) * 0x4) bits 6:4) field TX_CLK_EN of register PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    field_value = (reg_value & DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_MSK)
                  >> DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_MSK, DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_CLK_EN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_CLK_SEL_set", N, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_CLK_SEL_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_TX_CLK_SEL_set", N, value );

    /* ((0x00001060 + (N) * 0x4) bits 3:0) field TX_CLK_SEL of register PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11 index N=0..11 */
    sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                   h_ptr,
                                                                   N,
                                                                   DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_MSK,
                                                                   DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_OFF,
                                                                   value);
}

static INLINE UINT32 sys_dsis_field_TX_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_CLK_SEL_get", N, 11);
    /* ((0x00001060 + (N) * 0x4) bits 3:0) field TX_CLK_SEL of register PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    value = (reg_value & DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_MSK) >> DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_TX_CLK_SEL_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_CLK_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_CLK_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_CLK_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_CLK_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001060 + (N) * 0x4) bits 3:0) field TX_CLK_SEL of register PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11 index N=0..11 */
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
        /* ((0x00001060 + (N) * 0x4) bits 3:0) field TX_CLK_SEL of register PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11 index N=0..11 */
        sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                       h_ptr,
                                                                       N,
                                                                       subfield_mask << (DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_OFF + subfield_offset),
                                                                       DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_CLK_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_CLK_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_CLK_SEL_get", stop_bit, 3 );
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
    /* ((0x00001060 + (N) * 0x4) bits 3:0) field TX_CLK_SEL of register PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    field_value = (reg_value & DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_MSK)
                  >> DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_MSK, DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_CLK_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_I_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_I_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_I_CLK_SEL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_I_CLK_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_TX_I_CLK_SEL_set", N, value );

    /* ((0x00001090 + (N) * 0x4) bits 1:0) field TX_I_CLK_SEL of register PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11 index N=0..11 */
    sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                   h_ptr,
                                                                   N,
                                                                   DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_MSK,
                                                                   DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_OFF,
                                                                   value);
}

static INLINE UINT32 sys_dsis_field_TX_I_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_I_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_I_CLK_SEL_get", N, 11);
    /* ((0x00001090 + (N) * 0x4) bits 1:0) field TX_I_CLK_SEL of register PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    value = (reg_value & DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_MSK) >> DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_TX_I_CLK_SEL_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_I_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_I_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_I_CLK_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_I_CLK_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_I_CLK_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_I_CLK_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001090 + (N) * 0x4) bits 1:0) field TX_I_CLK_SEL of register PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11 index N=0..11 */
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
        /* ((0x00001090 + (N) * 0x4) bits 1:0) field TX_I_CLK_SEL of register PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11 index N=0..11 */
        sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                       h_ptr,
                                                                       N,
                                                                       subfield_mask << (DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_OFF + subfield_offset),
                                                                       DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_I_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_I_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_I_CLK_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_I_CLK_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_I_CLK_SEL_get", stop_bit, 1 );
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
    /* ((0x00001090 + (N) * 0x4) bits 1:0) field TX_I_CLK_SEL of register PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    field_value = (reg_value & DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_MSK)
                  >> DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_MSK, DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_I_CLK_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_ACB_IF_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_ACB_IF_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_ACB_IF_SEL_set", N, 11);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_ACB_IF_SEL_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_ACB_IF_SEL_set", N, value );

    /* ((0x000010c4 + (N) * 0x4) bits 6:4) field ACB_IF_SEL of register PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11 index N=0..11 */
    sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                   h_ptr,
                                                                   N,
                                                                   DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_MSK,
                                                                   DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_OFF,
                                                                   value);
}

static INLINE UINT32 sys_dsis_field_ACB_IF_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_ACB_IF_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_ACB_IF_SEL_get", N, 11);
    /* ((0x000010c4 + (N) * 0x4) bits 6:4) field ACB_IF_SEL of register PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    value = (reg_value & DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_MSK) >> DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_ACB_IF_SEL_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_ACB_IF_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_ACB_IF_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_ACB_IF_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_ACB_IF_SEL_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_ACB_IF_SEL_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_ACB_IF_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000010c4 + (N) * 0x4) bits 6:4) field ACB_IF_SEL of register PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11 index N=0..11 */
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
        /* ((0x000010c4 + (N) * 0x4) bits 6:4) field ACB_IF_SEL of register PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11 index N=0..11 */
        sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                       h_ptr,
                                                                       N,
                                                                       subfield_mask << (DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_OFF + subfield_offset),
                                                                       DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_ACB_IF_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_ACB_IF_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_ACB_IF_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_ACB_IF_SEL_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_ACB_IF_SEL_get", stop_bit, 2 );
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
    /* ((0x000010c4 + (N) * 0x4) bits 6:4) field ACB_IF_SEL of register PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    field_value = (reg_value & DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_MSK)
                  >> DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_MSK, DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_ACB_IF_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_IF_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_IF_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                 sifd_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_IF_SEL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_IF_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_TX_IF_SEL_set", N, value );

    /* ((0x000010c4 + (N) * 0x4) bits 1:0) field TX_IF_SEL of register PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11 index N=0..11 */
    sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                   h_ptr,
                                                                   N,
                                                                   DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_MSK,
                                                                   DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_OFF,
                                                                   value);
}

static INLINE UINT32 sys_dsis_field_TX_IF_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_IF_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_IF_SEL_get", N, 11);
    /* ((0x000010c4 + (N) * 0x4) bits 1:0) field TX_IF_SEL of register PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    value = (reg_value & DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_MSK) >> DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_TX_IF_SEL_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_IF_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_IF_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_IF_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_IF_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_IF_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_IF_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000010c4 + (N) * 0x4) bits 1:0) field TX_IF_SEL of register PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11 index N=0..11 */
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
        /* ((0x000010c4 + (N) * 0x4) bits 1:0) field TX_IF_SEL of register PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11 index N=0..11 */
        sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_field_set( b_ptr,
                                                                       h_ptr,
                                                                       N,
                                                                       subfield_mask << (DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_OFF + subfield_offset),
                                                                       DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_IF_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_IF_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_IF_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_IF_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_IF_SEL_get", stop_bit, 1 );
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
    /* ((0x000010c4 + (N) * 0x4) bits 1:0) field TX_IF_SEL of register PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    field_value = (reg_value & DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_MSK)
                  >> DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_MSK, DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_IF_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_SEL_set( sys_dsis_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_SEL_set( sys_dsis_buffer_t *b_ptr,
                                              sifd_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_SEL_set", N, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_SEL_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_TX_SEL_set", N, value );

    /* ((0x000010f4 + (N) * 0x4) bits 3:0) field TX_SEL of register PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11 index N=0..11 */
    sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_MSK,
                                                               DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_OFF,
                                                               value);
}

static INLINE UINT32 sys_dsis_field_TX_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                sifd_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_SEL_get", N, 11);
    /* ((0x000010f4 + (N) * 0x4) bits 3:0) field TX_SEL of register PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_read( b_ptr,
                                                                      h_ptr,
                                                                      N);
    value = (reg_value & DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_MSK) >> DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_TX_SEL_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000010f4 + (N) * 0x4) bits 3:0) field TX_SEL of register PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11 index N=0..11 */
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
        /* ((0x000010f4 + (N) * 0x4) bits 3:0) field TX_SEL of register PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11 index N=0..11 */
        sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_field_set( b_ptr,
                                                                   h_ptr,
                                                                   N,
                                                                   subfield_mask << (DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_OFF + subfield_offset),
                                                                   DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_SEL_get", stop_bit, 3 );
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
    /* ((0x000010f4 + (N) * 0x4) bits 3:0) field TX_SEL of register PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11 index N=0..11 */
    reg_value = sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_read( b_ptr,
                                                                      h_ptr,
                                                                      N);
    field_value = (reg_value & DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_MSK)
                  >> DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_MSK, DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_set", N, 11);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_set", N, value );

    /* ((0x00001148 + (N) * 0x4) bits 4:0) field TX_FIFO_WRITE_CLK_SEL of register PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT index N=0..11 */
    sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_MSK,
                                                           DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_OFF,
                                                           value);
}

static INLINE UINT32 sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_get", N, 11);
    /* ((0x00001148 + (N) * 0x4) bits 4:0) field TX_FIFO_WRITE_CLK_SEL of register PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT index N=0..11 */
    reg_value = sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_MSK) >> DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_set( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001148 + (N) * 0x4) bits 4:0) field TX_FIFO_WRITE_CLK_SEL of register PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT index N=0..11 */
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
        /* ((0x00001148 + (N) * 0x4) bits 4:0) field TX_FIFO_WRITE_CLK_SEL of register PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT index N=0..11 */
        sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               subfield_mask << (DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_OFF + subfield_offset),
                                                               DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_get( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_get", stop_bit, 4 );
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
    /* ((0x00001148 + (N) * 0x4) bits 4:0) field TX_FIFO_WRITE_CLK_SEL of register PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT index N=0..11 */
    reg_value = sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    field_value = (reg_value & DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_MSK)
                  >> DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_MSK, DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sys_dsis_field_TX_FIFO_READ_PTR_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_TX_FIFO_READ_PTR_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_FIFO_READ_PTR_set", N, 11);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_TX_FIFO_READ_PTR_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "sys_dsis_field_TX_FIFO_READ_PTR_set", N, value );

    /* ((0x000011a8 + (N) * 0x4) bits 2:0) field TX_FIFO_READ_PTR of register PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO index N=0..11 */
    sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_field_set( b_ptr,
                                                                  h_ptr,
                                                                  N,
                                                                  DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_BIT_TX_FIFO_READ_PTR_MSK,
                                                                  DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_BIT_TX_FIFO_READ_PTR_OFF,
                                                                  value);
}

static INLINE UINT32 sys_dsis_field_TX_FIFO_READ_PTR_get( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_FIFO_READ_PTR_get( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_TX_FIFO_READ_PTR_get", N, 11);
    /* ((0x000011a8 + (N) * 0x4) bits 2:0) field TX_FIFO_READ_PTR of register PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO index N=0..11 */
    reg_value = sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_read( b_ptr,
                                                                         h_ptr,
                                                                         N);
    value = (reg_value & DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_BIT_TX_FIFO_READ_PTR_MSK) >> DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_BIT_TX_FIFO_READ_PTR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sys_dsis_field_TX_FIFO_READ_PTR_get", N, value );

    return value;
}
static INLINE void sys_dsis_field_range_TX_FIFO_READ_PTR_set( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_range_TX_FIFO_READ_PTR_set( sys_dsis_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_FIFO_READ_PTR_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_READ_PTR_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_READ_PTR_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_READ_PTR_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000011a8 + (N) * 0x4) bits 2:0) field TX_FIFO_READ_PTR of register PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO index N=0..11 */
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
        /* ((0x000011a8 + (N) * 0x4) bits 2:0) field TX_FIFO_READ_PTR of register PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO index N=0..11 */
        sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_field_set( b_ptr,
                                                                      h_ptr,
                                                                      N,
                                                                      subfield_mask << (DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_BIT_TX_FIFO_READ_PTR_OFF + subfield_offset),
                                                                      DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_BIT_TX_FIFO_READ_PTR_OFF + subfield_offset,
                                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 sys_dsis_field_range_TX_FIFO_READ_PTR_get( sys_dsis_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_FIFO_READ_PTR_get( sys_dsis_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sys_dsis_field_range_TX_FIFO_READ_PTR_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_READ_PTR_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_READ_PTR_get", stop_bit, 2 );
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
    /* ((0x000011a8 + (N) * 0x4) bits 2:0) field TX_FIFO_READ_PTR of register PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO index N=0..11 */
    reg_value = sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_read( b_ptr,
                                                                         h_ptr,
                                                                         N);
    field_value = (reg_value & DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_BIT_TX_FIFO_READ_PTR_MSK)
                  >> DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_BIT_TX_FIFO_READ_PTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_BIT_TX_FIFO_READ_PTR_MSK, DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_BIT_TX_FIFO_READ_PTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_READ_PTR_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void sys_dsis_field_RP_NOT_CENTERED_0_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_0_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_0_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_0_E_set", value );

    /* (0x00001248 bits 4) field RP_NOT_CENTERED_0_E of register PMC_DSIS_REG_TX_FIFO_0_INT_EN */
    sys_dsis_reg_TX_FIFO_0_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_NOT_CENTERED_0_E_MSK,
                                             DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_NOT_CENTERED_0_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_0_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_0_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001248 bits 4) field RP_NOT_CENTERED_0_E of register PMC_DSIS_REG_TX_FIFO_0_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_0_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_NOT_CENTERED_0_E_MSK) >> DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_NOT_CENTERED_0_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_0_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_0_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_0_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_0_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_0_E_set", value );

    /* (0x00001248 bits 3) field RP_OUT_OF_SYNC_0_E of register PMC_DSIS_REG_TX_FIFO_0_INT_EN */
    sys_dsis_reg_TX_FIFO_0_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_OUT_OF_SYNC_0_E_MSK,
                                             DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_OUT_OF_SYNC_0_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_0_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_0_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001248 bits 3) field RP_OUT_OF_SYNC_0_E of register PMC_DSIS_REG_TX_FIFO_0_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_0_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_OUT_OF_SYNC_0_E_MSK) >> DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_OUT_OF_SYNC_0_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_0_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_0_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_0_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_0_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_0_E_set", value );

    /* (0x00001248 bits 2) field RP_ABOVE_EXP_0_E of register PMC_DSIS_REG_TX_FIFO_0_INT_EN */
    sys_dsis_reg_TX_FIFO_0_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_ABOVE_EXP_0_E_MSK,
                                             DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_ABOVE_EXP_0_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_0_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_0_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001248 bits 2) field RP_ABOVE_EXP_0_E of register PMC_DSIS_REG_TX_FIFO_0_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_0_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_ABOVE_EXP_0_E_MSK) >> DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_ABOVE_EXP_0_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_0_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_0_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_0_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_0_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_0_E_set", value );

    /* (0x00001248 bits 1) field RP_BELOW_EXP_0_E of register PMC_DSIS_REG_TX_FIFO_0_INT_EN */
    sys_dsis_reg_TX_FIFO_0_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_BELOW_EXP_0_E_MSK,
                                             DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_BELOW_EXP_0_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_0_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_0_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001248 bits 1) field RP_BELOW_EXP_0_E of register PMC_DSIS_REG_TX_FIFO_0_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_0_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_BELOW_EXP_0_E_MSK) >> DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_BELOW_EXP_0_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_0_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_0_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_0_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_0_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_0_E_set", value );

    /* (0x00001248 bits 0) field RP_IN_SYNC_0_E of register PMC_DSIS_REG_TX_FIFO_0_INT_EN */
    sys_dsis_reg_TX_FIFO_0_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_IN_SYNC_0_E_MSK,
                                             DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_IN_SYNC_0_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_0_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_0_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001248 bits 0) field RP_IN_SYNC_0_E of register PMC_DSIS_REG_TX_FIFO_0_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_0_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_IN_SYNC_0_E_MSK) >> DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_IN_SYNC_0_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_0_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_NOT_CENTERED_1_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_1_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_1_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_1_E_set", value );

    /* (0x0000124c bits 4) field RP_NOT_CENTERED_1_E of register PMC_DSIS_REG_TX_FIFO_1_INT_EN */
    sys_dsis_reg_TX_FIFO_1_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_NOT_CENTERED_1_E_MSK,
                                             DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_NOT_CENTERED_1_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_1_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_1_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000124c bits 4) field RP_NOT_CENTERED_1_E of register PMC_DSIS_REG_TX_FIFO_1_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_1_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_NOT_CENTERED_1_E_MSK) >> DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_NOT_CENTERED_1_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_1_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_1_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_1_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_1_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_1_E_set", value );

    /* (0x0000124c bits 3) field RP_OUT_OF_SYNC_1_E of register PMC_DSIS_REG_TX_FIFO_1_INT_EN */
    sys_dsis_reg_TX_FIFO_1_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_OUT_OF_SYNC_1_E_MSK,
                                             DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_OUT_OF_SYNC_1_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_1_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_1_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000124c bits 3) field RP_OUT_OF_SYNC_1_E of register PMC_DSIS_REG_TX_FIFO_1_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_1_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_OUT_OF_SYNC_1_E_MSK) >> DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_OUT_OF_SYNC_1_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_1_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_1_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_1_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_1_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_1_E_set", value );

    /* (0x0000124c bits 2) field RP_ABOVE_EXP_1_E of register PMC_DSIS_REG_TX_FIFO_1_INT_EN */
    sys_dsis_reg_TX_FIFO_1_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_ABOVE_EXP_1_E_MSK,
                                             DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_ABOVE_EXP_1_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_1_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_1_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000124c bits 2) field RP_ABOVE_EXP_1_E of register PMC_DSIS_REG_TX_FIFO_1_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_1_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_ABOVE_EXP_1_E_MSK) >> DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_ABOVE_EXP_1_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_1_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_1_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_1_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_1_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_1_E_set", value );

    /* (0x0000124c bits 1) field RP_BELOW_EXP_1_E of register PMC_DSIS_REG_TX_FIFO_1_INT_EN */
    sys_dsis_reg_TX_FIFO_1_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_BELOW_EXP_1_E_MSK,
                                             DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_BELOW_EXP_1_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_1_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_1_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000124c bits 1) field RP_BELOW_EXP_1_E of register PMC_DSIS_REG_TX_FIFO_1_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_1_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_BELOW_EXP_1_E_MSK) >> DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_BELOW_EXP_1_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_1_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_1_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_1_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_1_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_1_E_set", value );

    /* (0x0000124c bits 0) field RP_IN_SYNC_1_E of register PMC_DSIS_REG_TX_FIFO_1_INT_EN */
    sys_dsis_reg_TX_FIFO_1_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_IN_SYNC_1_E_MSK,
                                             DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_IN_SYNC_1_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_1_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_1_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000124c bits 0) field RP_IN_SYNC_1_E of register PMC_DSIS_REG_TX_FIFO_1_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_1_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_IN_SYNC_1_E_MSK) >> DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_IN_SYNC_1_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_1_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_NOT_CENTERED_2_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_2_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_2_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_2_E_set", value );

    /* (0x00001250 bits 4) field RP_NOT_CENTERED_2_E of register PMC_DSIS_REG_TX_FIFO_2_INT_EN */
    sys_dsis_reg_TX_FIFO_2_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_NOT_CENTERED_2_E_MSK,
                                             DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_NOT_CENTERED_2_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_2_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_2_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001250 bits 4) field RP_NOT_CENTERED_2_E of register PMC_DSIS_REG_TX_FIFO_2_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_2_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_NOT_CENTERED_2_E_MSK) >> DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_NOT_CENTERED_2_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_2_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_2_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_2_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_2_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_2_E_set", value );

    /* (0x00001250 bits 3) field RP_OUT_OF_SYNC_2_E of register PMC_DSIS_REG_TX_FIFO_2_INT_EN */
    sys_dsis_reg_TX_FIFO_2_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_OUT_OF_SYNC_2_E_MSK,
                                             DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_OUT_OF_SYNC_2_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_2_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_2_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001250 bits 3) field RP_OUT_OF_SYNC_2_E of register PMC_DSIS_REG_TX_FIFO_2_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_2_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_OUT_OF_SYNC_2_E_MSK) >> DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_OUT_OF_SYNC_2_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_2_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_2_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_2_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_2_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_2_E_set", value );

    /* (0x00001250 bits 2) field RP_ABOVE_EXP_2_E of register PMC_DSIS_REG_TX_FIFO_2_INT_EN */
    sys_dsis_reg_TX_FIFO_2_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_ABOVE_EXP_2_E_MSK,
                                             DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_ABOVE_EXP_2_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_2_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_2_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001250 bits 2) field RP_ABOVE_EXP_2_E of register PMC_DSIS_REG_TX_FIFO_2_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_2_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_ABOVE_EXP_2_E_MSK) >> DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_ABOVE_EXP_2_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_2_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_2_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_2_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_2_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_2_E_set", value );

    /* (0x00001250 bits 1) field RP_BELOW_EXP_2_E of register PMC_DSIS_REG_TX_FIFO_2_INT_EN */
    sys_dsis_reg_TX_FIFO_2_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_BELOW_EXP_2_E_MSK,
                                             DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_BELOW_EXP_2_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_2_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_2_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001250 bits 1) field RP_BELOW_EXP_2_E of register PMC_DSIS_REG_TX_FIFO_2_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_2_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_BELOW_EXP_2_E_MSK) >> DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_BELOW_EXP_2_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_2_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_2_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_2_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_2_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_2_E_set", value );

    /* (0x00001250 bits 0) field RP_IN_SYNC_2_E of register PMC_DSIS_REG_TX_FIFO_2_INT_EN */
    sys_dsis_reg_TX_FIFO_2_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_IN_SYNC_2_E_MSK,
                                             DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_IN_SYNC_2_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_2_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_2_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001250 bits 0) field RP_IN_SYNC_2_E of register PMC_DSIS_REG_TX_FIFO_2_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_2_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_IN_SYNC_2_E_MSK) >> DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_IN_SYNC_2_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_2_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_NOT_CENTERED_3_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_3_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_3_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_3_E_set", value );

    /* (0x00001254 bits 4) field RP_NOT_CENTERED_3_E of register PMC_DSIS_REG_TX_FIFO_3_INT_EN */
    sys_dsis_reg_TX_FIFO_3_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_NOT_CENTERED_3_E_MSK,
                                             DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_NOT_CENTERED_3_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_3_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_3_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001254 bits 4) field RP_NOT_CENTERED_3_E of register PMC_DSIS_REG_TX_FIFO_3_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_3_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_NOT_CENTERED_3_E_MSK) >> DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_NOT_CENTERED_3_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_3_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_3_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_3_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_3_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_3_E_set", value );

    /* (0x00001254 bits 3) field RP_OUT_OF_SYNC_3_E of register PMC_DSIS_REG_TX_FIFO_3_INT_EN */
    sys_dsis_reg_TX_FIFO_3_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_OUT_OF_SYNC_3_E_MSK,
                                             DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_OUT_OF_SYNC_3_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_3_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_3_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001254 bits 3) field RP_OUT_OF_SYNC_3_E of register PMC_DSIS_REG_TX_FIFO_3_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_3_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_OUT_OF_SYNC_3_E_MSK) >> DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_OUT_OF_SYNC_3_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_3_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_3_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_3_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_3_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_3_E_set", value );

    /* (0x00001254 bits 2) field RP_ABOVE_EXP_3_E of register PMC_DSIS_REG_TX_FIFO_3_INT_EN */
    sys_dsis_reg_TX_FIFO_3_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_ABOVE_EXP_3_E_MSK,
                                             DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_ABOVE_EXP_3_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_3_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_3_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001254 bits 2) field RP_ABOVE_EXP_3_E of register PMC_DSIS_REG_TX_FIFO_3_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_3_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_ABOVE_EXP_3_E_MSK) >> DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_ABOVE_EXP_3_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_3_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_3_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_3_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_3_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_3_E_set", value );

    /* (0x00001254 bits 1) field RP_BELOW_EXP_3_E of register PMC_DSIS_REG_TX_FIFO_3_INT_EN */
    sys_dsis_reg_TX_FIFO_3_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_BELOW_EXP_3_E_MSK,
                                             DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_BELOW_EXP_3_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_3_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_3_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001254 bits 1) field RP_BELOW_EXP_3_E of register PMC_DSIS_REG_TX_FIFO_3_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_3_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_BELOW_EXP_3_E_MSK) >> DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_BELOW_EXP_3_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_3_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_3_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_3_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_3_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_3_E_set", value );

    /* (0x00001254 bits 0) field RP_IN_SYNC_3_E of register PMC_DSIS_REG_TX_FIFO_3_INT_EN */
    sys_dsis_reg_TX_FIFO_3_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_IN_SYNC_3_E_MSK,
                                             DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_IN_SYNC_3_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_3_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_3_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001254 bits 0) field RP_IN_SYNC_3_E of register PMC_DSIS_REG_TX_FIFO_3_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_3_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_IN_SYNC_3_E_MSK) >> DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_IN_SYNC_3_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_3_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_NOT_CENTERED_4_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_4_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_4_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_4_E_set", value );

    /* (0x00001258 bits 4) field RP_NOT_CENTERED_4_E of register PMC_DSIS_REG_TX_FIFO_4_INT_EN */
    sys_dsis_reg_TX_FIFO_4_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_NOT_CENTERED_4_E_MSK,
                                             DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_NOT_CENTERED_4_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_4_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_4_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001258 bits 4) field RP_NOT_CENTERED_4_E of register PMC_DSIS_REG_TX_FIFO_4_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_4_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_NOT_CENTERED_4_E_MSK) >> DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_NOT_CENTERED_4_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_4_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_4_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_4_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_4_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_4_E_set", value );

    /* (0x00001258 bits 3) field RP_OUT_OF_SYNC_4_E of register PMC_DSIS_REG_TX_FIFO_4_INT_EN */
    sys_dsis_reg_TX_FIFO_4_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_OUT_OF_SYNC_4_E_MSK,
                                             DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_OUT_OF_SYNC_4_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_4_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_4_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001258 bits 3) field RP_OUT_OF_SYNC_4_E of register PMC_DSIS_REG_TX_FIFO_4_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_4_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_OUT_OF_SYNC_4_E_MSK) >> DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_OUT_OF_SYNC_4_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_4_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_4_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_4_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_4_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_4_E_set", value );

    /* (0x00001258 bits 2) field RP_ABOVE_EXP_4_E of register PMC_DSIS_REG_TX_FIFO_4_INT_EN */
    sys_dsis_reg_TX_FIFO_4_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_ABOVE_EXP_4_E_MSK,
                                             DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_ABOVE_EXP_4_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_4_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_4_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001258 bits 2) field RP_ABOVE_EXP_4_E of register PMC_DSIS_REG_TX_FIFO_4_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_4_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_ABOVE_EXP_4_E_MSK) >> DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_ABOVE_EXP_4_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_4_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_4_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_4_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_4_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_4_E_set", value );

    /* (0x00001258 bits 1) field RP_BELOW_EXP_4_E of register PMC_DSIS_REG_TX_FIFO_4_INT_EN */
    sys_dsis_reg_TX_FIFO_4_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_BELOW_EXP_4_E_MSK,
                                             DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_BELOW_EXP_4_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_4_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_4_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001258 bits 1) field RP_BELOW_EXP_4_E of register PMC_DSIS_REG_TX_FIFO_4_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_4_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_BELOW_EXP_4_E_MSK) >> DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_BELOW_EXP_4_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_4_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_4_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_4_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_4_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_4_E_set", value );

    /* (0x00001258 bits 0) field RP_IN_SYNC_4_E of register PMC_DSIS_REG_TX_FIFO_4_INT_EN */
    sys_dsis_reg_TX_FIFO_4_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_IN_SYNC_4_E_MSK,
                                             DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_IN_SYNC_4_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_4_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_4_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001258 bits 0) field RP_IN_SYNC_4_E of register PMC_DSIS_REG_TX_FIFO_4_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_4_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_IN_SYNC_4_E_MSK) >> DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_IN_SYNC_4_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_4_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_NOT_CENTERED_5_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_5_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_5_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_5_E_set", value );

    /* (0x0000125c bits 4) field RP_NOT_CENTERED_5_E of register PMC_DSIS_REG_TX_FIFO_5_INT_EN */
    sys_dsis_reg_TX_FIFO_5_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_NOT_CENTERED_5_E_MSK,
                                             DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_NOT_CENTERED_5_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_5_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_5_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000125c bits 4) field RP_NOT_CENTERED_5_E of register PMC_DSIS_REG_TX_FIFO_5_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_5_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_NOT_CENTERED_5_E_MSK) >> DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_NOT_CENTERED_5_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_5_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_5_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_5_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_5_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_5_E_set", value );

    /* (0x0000125c bits 3) field RP_OUT_OF_SYNC_5_E of register PMC_DSIS_REG_TX_FIFO_5_INT_EN */
    sys_dsis_reg_TX_FIFO_5_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_OUT_OF_SYNC_5_E_MSK,
                                             DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_OUT_OF_SYNC_5_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_5_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_5_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000125c bits 3) field RP_OUT_OF_SYNC_5_E of register PMC_DSIS_REG_TX_FIFO_5_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_5_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_OUT_OF_SYNC_5_E_MSK) >> DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_OUT_OF_SYNC_5_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_5_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_5_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_5_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_5_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_5_E_set", value );

    /* (0x0000125c bits 2) field RP_ABOVE_EXP_5_E of register PMC_DSIS_REG_TX_FIFO_5_INT_EN */
    sys_dsis_reg_TX_FIFO_5_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_ABOVE_EXP_5_E_MSK,
                                             DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_ABOVE_EXP_5_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_5_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_5_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000125c bits 2) field RP_ABOVE_EXP_5_E of register PMC_DSIS_REG_TX_FIFO_5_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_5_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_ABOVE_EXP_5_E_MSK) >> DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_ABOVE_EXP_5_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_5_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_5_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_5_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_5_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_5_E_set", value );

    /* (0x0000125c bits 1) field RP_BELOW_EXP_5_E of register PMC_DSIS_REG_TX_FIFO_5_INT_EN */
    sys_dsis_reg_TX_FIFO_5_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_BELOW_EXP_5_E_MSK,
                                             DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_BELOW_EXP_5_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_5_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_5_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000125c bits 1) field RP_BELOW_EXP_5_E of register PMC_DSIS_REG_TX_FIFO_5_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_5_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_BELOW_EXP_5_E_MSK) >> DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_BELOW_EXP_5_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_5_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_5_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_5_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_5_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_5_E_set", value );

    /* (0x0000125c bits 0) field RP_IN_SYNC_5_E of register PMC_DSIS_REG_TX_FIFO_5_INT_EN */
    sys_dsis_reg_TX_FIFO_5_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_IN_SYNC_5_E_MSK,
                                             DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_IN_SYNC_5_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_5_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_5_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000125c bits 0) field RP_IN_SYNC_5_E of register PMC_DSIS_REG_TX_FIFO_5_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_5_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_IN_SYNC_5_E_MSK) >> DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_IN_SYNC_5_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_5_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_NOT_CENTERED_6_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_6_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_6_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_6_E_set", value );

    /* (0x00001260 bits 4) field RP_NOT_CENTERED_6_E of register PMC_DSIS_REG_TX_FIFO_6_INT_EN */
    sys_dsis_reg_TX_FIFO_6_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_NOT_CENTERED_6_E_MSK,
                                             DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_NOT_CENTERED_6_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_6_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_6_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001260 bits 4) field RP_NOT_CENTERED_6_E of register PMC_DSIS_REG_TX_FIFO_6_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_6_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_NOT_CENTERED_6_E_MSK) >> DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_NOT_CENTERED_6_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_6_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_6_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_6_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_6_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_6_E_set", value );

    /* (0x00001260 bits 3) field RP_OUT_OF_SYNC_6_E of register PMC_DSIS_REG_TX_FIFO_6_INT_EN */
    sys_dsis_reg_TX_FIFO_6_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_OUT_OF_SYNC_6_E_MSK,
                                             DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_OUT_OF_SYNC_6_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_6_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_6_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001260 bits 3) field RP_OUT_OF_SYNC_6_E of register PMC_DSIS_REG_TX_FIFO_6_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_6_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_OUT_OF_SYNC_6_E_MSK) >> DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_OUT_OF_SYNC_6_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_6_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_6_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_6_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_6_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_6_E_set", value );

    /* (0x00001260 bits 2) field RP_ABOVE_EXP_6_E of register PMC_DSIS_REG_TX_FIFO_6_INT_EN */
    sys_dsis_reg_TX_FIFO_6_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_ABOVE_EXP_6_E_MSK,
                                             DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_ABOVE_EXP_6_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_6_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_6_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001260 bits 2) field RP_ABOVE_EXP_6_E of register PMC_DSIS_REG_TX_FIFO_6_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_6_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_ABOVE_EXP_6_E_MSK) >> DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_ABOVE_EXP_6_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_6_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_6_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_6_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_6_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_6_E_set", value );

    /* (0x00001260 bits 1) field RP_BELOW_EXP_6_E of register PMC_DSIS_REG_TX_FIFO_6_INT_EN */
    sys_dsis_reg_TX_FIFO_6_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_BELOW_EXP_6_E_MSK,
                                             DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_BELOW_EXP_6_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_6_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_6_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001260 bits 1) field RP_BELOW_EXP_6_E of register PMC_DSIS_REG_TX_FIFO_6_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_6_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_BELOW_EXP_6_E_MSK) >> DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_BELOW_EXP_6_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_6_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_6_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_6_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_6_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_6_E_set", value );

    /* (0x00001260 bits 0) field RP_IN_SYNC_6_E of register PMC_DSIS_REG_TX_FIFO_6_INT_EN */
    sys_dsis_reg_TX_FIFO_6_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_IN_SYNC_6_E_MSK,
                                             DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_IN_SYNC_6_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_6_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_6_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001260 bits 0) field RP_IN_SYNC_6_E of register PMC_DSIS_REG_TX_FIFO_6_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_6_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_IN_SYNC_6_E_MSK) >> DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_IN_SYNC_6_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_6_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_NOT_CENTERED_7_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_7_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_7_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_7_E_set", value );

    /* (0x00001264 bits 4) field RP_NOT_CENTERED_7_E of register PMC_DSIS_REG_TX_FIFO_7_INT_EN */
    sys_dsis_reg_TX_FIFO_7_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_NOT_CENTERED_7_E_MSK,
                                             DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_NOT_CENTERED_7_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_7_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_7_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001264 bits 4) field RP_NOT_CENTERED_7_E of register PMC_DSIS_REG_TX_FIFO_7_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_7_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_NOT_CENTERED_7_E_MSK) >> DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_NOT_CENTERED_7_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_7_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_7_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_7_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_7_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_7_E_set", value );

    /* (0x00001264 bits 3) field RP_OUT_OF_SYNC_7_E of register PMC_DSIS_REG_TX_FIFO_7_INT_EN */
    sys_dsis_reg_TX_FIFO_7_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_OUT_OF_SYNC_7_E_MSK,
                                             DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_OUT_OF_SYNC_7_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_7_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_7_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001264 bits 3) field RP_OUT_OF_SYNC_7_E of register PMC_DSIS_REG_TX_FIFO_7_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_7_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_OUT_OF_SYNC_7_E_MSK) >> DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_OUT_OF_SYNC_7_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_7_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_7_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_7_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_7_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_7_E_set", value );

    /* (0x00001264 bits 2) field RP_ABOVE_EXP_7_E of register PMC_DSIS_REG_TX_FIFO_7_INT_EN */
    sys_dsis_reg_TX_FIFO_7_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_ABOVE_EXP_7_E_MSK,
                                             DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_ABOVE_EXP_7_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_7_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_7_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001264 bits 2) field RP_ABOVE_EXP_7_E of register PMC_DSIS_REG_TX_FIFO_7_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_7_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_ABOVE_EXP_7_E_MSK) >> DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_ABOVE_EXP_7_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_7_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_7_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_7_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_7_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_7_E_set", value );

    /* (0x00001264 bits 1) field RP_BELOW_EXP_7_E of register PMC_DSIS_REG_TX_FIFO_7_INT_EN */
    sys_dsis_reg_TX_FIFO_7_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_BELOW_EXP_7_E_MSK,
                                             DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_BELOW_EXP_7_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_7_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_7_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001264 bits 1) field RP_BELOW_EXP_7_E of register PMC_DSIS_REG_TX_FIFO_7_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_7_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_BELOW_EXP_7_E_MSK) >> DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_BELOW_EXP_7_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_7_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_7_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_7_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_7_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_7_E_set", value );

    /* (0x00001264 bits 0) field RP_IN_SYNC_7_E of register PMC_DSIS_REG_TX_FIFO_7_INT_EN */
    sys_dsis_reg_TX_FIFO_7_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_IN_SYNC_7_E_MSK,
                                             DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_IN_SYNC_7_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_7_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_7_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001264 bits 0) field RP_IN_SYNC_7_E of register PMC_DSIS_REG_TX_FIFO_7_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_7_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_IN_SYNC_7_E_MSK) >> DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_IN_SYNC_7_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_7_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_NOT_CENTERED_8_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_8_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_8_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_8_E_set", value );

    /* (0x00001268 bits 4) field RP_NOT_CENTERED_8_E of register PMC_DSIS_REG_TX_FIFO_8_INT_EN */
    sys_dsis_reg_TX_FIFO_8_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_NOT_CENTERED_8_E_MSK,
                                             DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_NOT_CENTERED_8_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_8_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_8_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001268 bits 4) field RP_NOT_CENTERED_8_E of register PMC_DSIS_REG_TX_FIFO_8_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_8_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_NOT_CENTERED_8_E_MSK) >> DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_NOT_CENTERED_8_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_8_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_8_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_8_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_8_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_8_E_set", value );

    /* (0x00001268 bits 3) field RP_OUT_OF_SYNC_8_E of register PMC_DSIS_REG_TX_FIFO_8_INT_EN */
    sys_dsis_reg_TX_FIFO_8_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_OUT_OF_SYNC_8_E_MSK,
                                             DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_OUT_OF_SYNC_8_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_8_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_8_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001268 bits 3) field RP_OUT_OF_SYNC_8_E of register PMC_DSIS_REG_TX_FIFO_8_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_8_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_OUT_OF_SYNC_8_E_MSK) >> DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_OUT_OF_SYNC_8_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_8_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_8_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_8_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_8_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_8_E_set", value );

    /* (0x00001268 bits 2) field RP_ABOVE_EXP_8_E of register PMC_DSIS_REG_TX_FIFO_8_INT_EN */
    sys_dsis_reg_TX_FIFO_8_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_ABOVE_EXP_8_E_MSK,
                                             DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_ABOVE_EXP_8_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_8_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_8_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001268 bits 2) field RP_ABOVE_EXP_8_E of register PMC_DSIS_REG_TX_FIFO_8_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_8_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_ABOVE_EXP_8_E_MSK) >> DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_ABOVE_EXP_8_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_8_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_8_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_8_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_8_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_8_E_set", value );

    /* (0x00001268 bits 1) field RP_BELOW_EXP_8_E of register PMC_DSIS_REG_TX_FIFO_8_INT_EN */
    sys_dsis_reg_TX_FIFO_8_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_BELOW_EXP_8_E_MSK,
                                             DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_BELOW_EXP_8_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_8_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_8_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001268 bits 1) field RP_BELOW_EXP_8_E of register PMC_DSIS_REG_TX_FIFO_8_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_8_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_BELOW_EXP_8_E_MSK) >> DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_BELOW_EXP_8_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_8_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_8_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_8_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_8_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_8_E_set", value );

    /* (0x00001268 bits 0) field RP_IN_SYNC_8_E of register PMC_DSIS_REG_TX_FIFO_8_INT_EN */
    sys_dsis_reg_TX_FIFO_8_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_IN_SYNC_8_E_MSK,
                                             DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_IN_SYNC_8_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_8_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_8_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001268 bits 0) field RP_IN_SYNC_8_E of register PMC_DSIS_REG_TX_FIFO_8_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_8_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_IN_SYNC_8_E_MSK) >> DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_IN_SYNC_8_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_8_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_NOT_CENTERED_9_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_9_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_9_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_9_E_set", value );

    /* (0x0000126c bits 4) field RP_NOT_CENTERED_9_E of register PMC_DSIS_REG_TX_FIFO_9_INT_EN */
    sys_dsis_reg_TX_FIFO_9_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_NOT_CENTERED_9_E_MSK,
                                             DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_NOT_CENTERED_9_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_9_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_9_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000126c bits 4) field RP_NOT_CENTERED_9_E of register PMC_DSIS_REG_TX_FIFO_9_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_9_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_NOT_CENTERED_9_E_MSK) >> DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_NOT_CENTERED_9_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_9_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_9_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_9_E_set( sys_dsis_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_9_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_9_E_set", value );

    /* (0x0000126c bits 3) field RP_OUT_OF_SYNC_9_E of register PMC_DSIS_REG_TX_FIFO_9_INT_EN */
    sys_dsis_reg_TX_FIFO_9_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_OUT_OF_SYNC_9_E_MSK,
                                             DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_OUT_OF_SYNC_9_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_9_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_9_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000126c bits 3) field RP_OUT_OF_SYNC_9_E of register PMC_DSIS_REG_TX_FIFO_9_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_9_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_OUT_OF_SYNC_9_E_MSK) >> DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_OUT_OF_SYNC_9_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_9_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_9_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_9_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_9_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_9_E_set", value );

    /* (0x0000126c bits 2) field RP_ABOVE_EXP_9_E of register PMC_DSIS_REG_TX_FIFO_9_INT_EN */
    sys_dsis_reg_TX_FIFO_9_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_ABOVE_EXP_9_E_MSK,
                                             DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_ABOVE_EXP_9_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_9_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_9_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000126c bits 2) field RP_ABOVE_EXP_9_E of register PMC_DSIS_REG_TX_FIFO_9_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_9_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_ABOVE_EXP_9_E_MSK) >> DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_ABOVE_EXP_9_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_9_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_9_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_9_E_set( sys_dsis_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_9_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_9_E_set", value );

    /* (0x0000126c bits 1) field RP_BELOW_EXP_9_E of register PMC_DSIS_REG_TX_FIFO_9_INT_EN */
    sys_dsis_reg_TX_FIFO_9_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_BELOW_EXP_9_E_MSK,
                                             DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_BELOW_EXP_9_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_9_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_9_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000126c bits 1) field RP_BELOW_EXP_9_E of register PMC_DSIS_REG_TX_FIFO_9_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_9_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_BELOW_EXP_9_E_MSK) >> DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_BELOW_EXP_9_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_9_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_9_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_9_E_set( sys_dsis_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_9_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_9_E_set", value );

    /* (0x0000126c bits 0) field RP_IN_SYNC_9_E of register PMC_DSIS_REG_TX_FIFO_9_INT_EN */
    sys_dsis_reg_TX_FIFO_9_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_IN_SYNC_9_E_MSK,
                                             DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_IN_SYNC_9_E_OFF,
                                             value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_9_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_9_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000126c bits 0) field RP_IN_SYNC_9_E of register PMC_DSIS_REG_TX_FIFO_9_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_9_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_IN_SYNC_9_E_MSK) >> DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_IN_SYNC_9_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_9_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_NOT_CENTERED_10_E_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_10_E_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_10_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_10_E_set", value );

    /* (0x00001270 bits 4) field RP_NOT_CENTERED_10_E of register PMC_DSIS_REG_TX_FIFO_10_INT_EN */
    sys_dsis_reg_TX_FIFO_10_INT_EN_field_set( b_ptr,
                                              h_ptr,
                                              DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_NOT_CENTERED_10_E_MSK,
                                              DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_NOT_CENTERED_10_E_OFF,
                                              value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_10_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_10_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001270 bits 4) field RP_NOT_CENTERED_10_E of register PMC_DSIS_REG_TX_FIFO_10_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_10_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_NOT_CENTERED_10_E_MSK) >> DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_NOT_CENTERED_10_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_10_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_10_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_10_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_10_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_10_E_set", value );

    /* (0x00001270 bits 3) field RP_OUT_OF_SYNC_10_E of register PMC_DSIS_REG_TX_FIFO_10_INT_EN */
    sys_dsis_reg_TX_FIFO_10_INT_EN_field_set( b_ptr,
                                              h_ptr,
                                              DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_OUT_OF_SYNC_10_E_MSK,
                                              DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_OUT_OF_SYNC_10_E_OFF,
                                              value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_10_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_10_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001270 bits 3) field RP_OUT_OF_SYNC_10_E of register PMC_DSIS_REG_TX_FIFO_10_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_10_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_OUT_OF_SYNC_10_E_MSK) >> DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_OUT_OF_SYNC_10_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_10_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_10_E_set( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_10_E_set( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_10_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_10_E_set", value );

    /* (0x00001270 bits 2) field RP_ABOVE_EXP_10_E of register PMC_DSIS_REG_TX_FIFO_10_INT_EN */
    sys_dsis_reg_TX_FIFO_10_INT_EN_field_set( b_ptr,
                                              h_ptr,
                                              DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_ABOVE_EXP_10_E_MSK,
                                              DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_ABOVE_EXP_10_E_OFF,
                                              value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_10_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_10_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001270 bits 2) field RP_ABOVE_EXP_10_E of register PMC_DSIS_REG_TX_FIFO_10_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_10_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_ABOVE_EXP_10_E_MSK) >> DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_ABOVE_EXP_10_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_10_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_10_E_set( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_10_E_set( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_10_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_10_E_set", value );

    /* (0x00001270 bits 1) field RP_BELOW_EXP_10_E of register PMC_DSIS_REG_TX_FIFO_10_INT_EN */
    sys_dsis_reg_TX_FIFO_10_INT_EN_field_set( b_ptr,
                                              h_ptr,
                                              DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_BELOW_EXP_10_E_MSK,
                                              DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_BELOW_EXP_10_E_OFF,
                                              value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_10_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_10_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001270 bits 1) field RP_BELOW_EXP_10_E of register PMC_DSIS_REG_TX_FIFO_10_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_10_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_BELOW_EXP_10_E_MSK) >> DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_BELOW_EXP_10_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_10_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_10_E_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_10_E_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_10_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_10_E_set", value );

    /* (0x00001270 bits 0) field RP_IN_SYNC_10_E of register PMC_DSIS_REG_TX_FIFO_10_INT_EN */
    sys_dsis_reg_TX_FIFO_10_INT_EN_field_set( b_ptr,
                                              h_ptr,
                                              DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_IN_SYNC_10_E_MSK,
                                              DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_IN_SYNC_10_E_OFF,
                                              value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_10_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_10_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001270 bits 0) field RP_IN_SYNC_10_E of register PMC_DSIS_REG_TX_FIFO_10_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_10_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_IN_SYNC_10_E_MSK) >> DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_IN_SYNC_10_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_10_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_NOT_CENTERED_11_E_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_11_E_set( sys_dsis_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_11_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_11_E_set", value );

    /* (0x00001274 bits 4) field RP_NOT_CENTERED_11_E of register PMC_DSIS_REG_TX_FIFO_11_INT_EN */
    sys_dsis_reg_TX_FIFO_11_INT_EN_field_set( b_ptr,
                                              h_ptr,
                                              DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_NOT_CENTERED_11_E_MSK,
                                              DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_NOT_CENTERED_11_E_OFF,
                                              value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_11_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_11_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001274 bits 4) field RP_NOT_CENTERED_11_E of register PMC_DSIS_REG_TX_FIFO_11_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_11_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_NOT_CENTERED_11_E_MSK) >> DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_NOT_CENTERED_11_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_11_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_11_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_11_E_set( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_11_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_11_E_set", value );

    /* (0x00001274 bits 3) field RP_OUT_OF_SYNC_11_E of register PMC_DSIS_REG_TX_FIFO_11_INT_EN */
    sys_dsis_reg_TX_FIFO_11_INT_EN_field_set( b_ptr,
                                              h_ptr,
                                              DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_OUT_OF_SYNC_11_E_MSK,
                                              DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_OUT_OF_SYNC_11_E_OFF,
                                              value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_11_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_11_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001274 bits 3) field RP_OUT_OF_SYNC_11_E of register PMC_DSIS_REG_TX_FIFO_11_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_11_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_OUT_OF_SYNC_11_E_MSK) >> DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_OUT_OF_SYNC_11_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_11_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_ABOVE_EXP_11_E_set( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_11_E_set( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_11_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_11_E_set", value );

    /* (0x00001274 bits 2) field RP_ABOVE_EXP_11_E of register PMC_DSIS_REG_TX_FIFO_11_INT_EN */
    sys_dsis_reg_TX_FIFO_11_INT_EN_field_set( b_ptr,
                                              h_ptr,
                                              DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_ABOVE_EXP_11_E_MSK,
                                              DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_ABOVE_EXP_11_E_OFF,
                                              value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_11_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_11_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001274 bits 2) field RP_ABOVE_EXP_11_E of register PMC_DSIS_REG_TX_FIFO_11_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_11_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_ABOVE_EXP_11_E_MSK) >> DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_ABOVE_EXP_11_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_11_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_BELOW_EXP_11_E_set( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_11_E_set( sys_dsis_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_11_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_11_E_set", value );

    /* (0x00001274 bits 1) field RP_BELOW_EXP_11_E of register PMC_DSIS_REG_TX_FIFO_11_INT_EN */
    sys_dsis_reg_TX_FIFO_11_INT_EN_field_set( b_ptr,
                                              h_ptr,
                                              DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_BELOW_EXP_11_E_MSK,
                                              DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_BELOW_EXP_11_E_OFF,
                                              value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_11_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_11_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001274 bits 1) field RP_BELOW_EXP_11_E of register PMC_DSIS_REG_TX_FIFO_11_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_11_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_BELOW_EXP_11_E_MSK) >> DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_BELOW_EXP_11_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_11_E_get", value );

    return value;
}
static INLINE void sys_dsis_field_RP_IN_SYNC_11_E_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_11_E_set( sys_dsis_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_11_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_11_E_set", value );

    /* (0x00001274 bits 0) field RP_IN_SYNC_11_E of register PMC_DSIS_REG_TX_FIFO_11_INT_EN */
    sys_dsis_reg_TX_FIFO_11_INT_EN_field_set( b_ptr,
                                              h_ptr,
                                              DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_IN_SYNC_11_E_MSK,
                                              DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_IN_SYNC_11_E_OFF,
                                              value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_11_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_11_E_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001274 bits 0) field RP_IN_SYNC_11_E of register PMC_DSIS_REG_TX_FIFO_11_INT_EN */
    reg_value = sys_dsis_reg_TX_FIFO_11_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_IN_SYNC_11_E_MSK) >> DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_IN_SYNC_11_E_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_11_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_summary
 * ==================================================================================
 */
static INLINE UINT32 sys_dsis_field_TX_FIFO_INT_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_TX_FIFO_INT_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001298 bits 19:0) field TX_FIFO_INT of register PMC_DSIS_REG_INT_SUMMARY */
    reg_value = sys_dsis_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_MSK) >> DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_TX_FIFO_INT_get", value );

    return value;
}
static INLINE UINT32 sys_dsis_field_range_TX_FIFO_INT_get( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_range_TX_FIFO_INT_get( sys_dsis_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_INT_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_INT_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001298 bits 19:0) field TX_FIFO_INT of register PMC_DSIS_REG_INT_SUMMARY */
    reg_value = sys_dsis_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_MSK)
                  >> DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_MSK, DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_range_TX_FIFO_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_range_TX_FIFO_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sys_dsis_field_range_TX_FIFO_INT_poll", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sys_dsis_field_range_TX_FIFO_INT_poll", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sys_dsis_field_range_TX_FIFO_INT_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001298 bits 19:0) field TX_FIFO_INT of register PMC_DSIS_REG_INT_SUMMARY */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001298 bits 19:0) field TX_FIFO_INT of register PMC_DSIS_REG_INT_SUMMARY */
        return sys_dsis_reg_INT_SUMMARY_poll( b_ptr,
                                              h_ptr,
                                              subfield_mask << (DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_OFF + subfield_offset),
                                              value << (DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_OFF + subfield_offset),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_TX_FIFO_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_TX_FIFO_INT_poll( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_TX_FIFO_INT_poll", value );

    /* (0x00001298 bits 19:0) field TX_FIFO_INT of register PMC_DSIS_REG_INT_SUMMARY */
    return sys_dsis_reg_INT_SUMMARY_poll( b_ptr,
                                          h_ptr,
                                          DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_MSK,
                                          (value<<DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_OFF),
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
static INLINE void sys_dsis_field_RP_NOT_CENTERED_0_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_0_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_0_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_0_I_set_to_clear", value );

    /* (0x000011f8 bits 4) field RP_NOT_CENTERED_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    sys_dsis_reg_TX_FIFO_0_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_0_INT_BIT_RP_NOT_CENTERED_0_I_MSK,
                                                          DSIS_REG_TX_FIFO_0_INT_BIT_RP_NOT_CENTERED_0_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_0_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_0_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011f8 bits 4) field RP_NOT_CENTERED_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    reg_value = sys_dsis_reg_TX_FIFO_0_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_0_INT_BIT_RP_NOT_CENTERED_0_I_MSK) >> DSIS_REG_TX_FIFO_0_INT_BIT_RP_NOT_CENTERED_0_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_0_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_0_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_0_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_0_I_poll", value );

    /* (0x000011f8 bits 4) field RP_NOT_CENTERED_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    return sys_dsis_reg_TX_FIFO_0_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_0_INT_BIT_RP_NOT_CENTERED_0_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_0_INT_BIT_RP_NOT_CENTERED_0_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_0_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_0_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_0_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_0_I_set_to_clear", value );

    /* (0x000011f8 bits 3) field RP_OUT_OF_SYNC_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    sys_dsis_reg_TX_FIFO_0_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_0_INT_BIT_RP_OUT_OF_SYNC_0_I_MSK,
                                                          DSIS_REG_TX_FIFO_0_INT_BIT_RP_OUT_OF_SYNC_0_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_0_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_0_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011f8 bits 3) field RP_OUT_OF_SYNC_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    reg_value = sys_dsis_reg_TX_FIFO_0_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_0_INT_BIT_RP_OUT_OF_SYNC_0_I_MSK) >> DSIS_REG_TX_FIFO_0_INT_BIT_RP_OUT_OF_SYNC_0_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_0_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_0_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_0_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_0_I_poll", value );

    /* (0x000011f8 bits 3) field RP_OUT_OF_SYNC_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    return sys_dsis_reg_TX_FIFO_0_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_0_INT_BIT_RP_OUT_OF_SYNC_0_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_0_INT_BIT_RP_OUT_OF_SYNC_0_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_0_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_0_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_0_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_0_I_set_to_clear", value );

    /* (0x000011f8 bits 2) field RP_ABOVE_EXP_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    sys_dsis_reg_TX_FIFO_0_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_0_INT_BIT_RP_ABOVE_EXP_0_I_MSK,
                                                          DSIS_REG_TX_FIFO_0_INT_BIT_RP_ABOVE_EXP_0_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_0_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_0_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011f8 bits 2) field RP_ABOVE_EXP_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    reg_value = sys_dsis_reg_TX_FIFO_0_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_0_INT_BIT_RP_ABOVE_EXP_0_I_MSK) >> DSIS_REG_TX_FIFO_0_INT_BIT_RP_ABOVE_EXP_0_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_0_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_0_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_0_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_0_I_poll", value );

    /* (0x000011f8 bits 2) field RP_ABOVE_EXP_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    return sys_dsis_reg_TX_FIFO_0_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_0_INT_BIT_RP_ABOVE_EXP_0_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_0_INT_BIT_RP_ABOVE_EXP_0_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_0_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_0_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_0_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_0_I_set_to_clear", value );

    /* (0x000011f8 bits 1) field RP_BELOW_EXP_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    sys_dsis_reg_TX_FIFO_0_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_0_INT_BIT_RP_BELOW_EXP_0_I_MSK,
                                                          DSIS_REG_TX_FIFO_0_INT_BIT_RP_BELOW_EXP_0_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_0_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_0_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011f8 bits 1) field RP_BELOW_EXP_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    reg_value = sys_dsis_reg_TX_FIFO_0_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_0_INT_BIT_RP_BELOW_EXP_0_I_MSK) >> DSIS_REG_TX_FIFO_0_INT_BIT_RP_BELOW_EXP_0_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_0_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_0_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_0_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_0_I_poll", value );

    /* (0x000011f8 bits 1) field RP_BELOW_EXP_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    return sys_dsis_reg_TX_FIFO_0_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_0_INT_BIT_RP_BELOW_EXP_0_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_0_INT_BIT_RP_BELOW_EXP_0_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_0_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_0_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_0_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_0_I_set_to_clear", value );

    /* (0x000011f8 bits 0) field RP_IN_SYNC_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    sys_dsis_reg_TX_FIFO_0_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_0_INT_BIT_RP_IN_SYNC_0_I_MSK,
                                                          DSIS_REG_TX_FIFO_0_INT_BIT_RP_IN_SYNC_0_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_0_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_0_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011f8 bits 0) field RP_IN_SYNC_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    reg_value = sys_dsis_reg_TX_FIFO_0_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_0_INT_BIT_RP_IN_SYNC_0_I_MSK) >> DSIS_REG_TX_FIFO_0_INT_BIT_RP_IN_SYNC_0_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_0_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_0_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_0_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_0_I_poll", value );

    /* (0x000011f8 bits 0) field RP_IN_SYNC_0_I of register PMC_DSIS_REG_TX_FIFO_0_INT */
    return sys_dsis_reg_TX_FIFO_0_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_0_INT_BIT_RP_IN_SYNC_0_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_0_INT_BIT_RP_IN_SYNC_0_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_NOT_CENTERED_1_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_1_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_1_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_1_I_set_to_clear", value );

    /* (0x000011fc bits 4) field RP_NOT_CENTERED_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    sys_dsis_reg_TX_FIFO_1_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_1_INT_BIT_RP_NOT_CENTERED_1_I_MSK,
                                                          DSIS_REG_TX_FIFO_1_INT_BIT_RP_NOT_CENTERED_1_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_1_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_1_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011fc bits 4) field RP_NOT_CENTERED_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    reg_value = sys_dsis_reg_TX_FIFO_1_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_1_INT_BIT_RP_NOT_CENTERED_1_I_MSK) >> DSIS_REG_TX_FIFO_1_INT_BIT_RP_NOT_CENTERED_1_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_1_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_1_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_1_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_1_I_poll", value );

    /* (0x000011fc bits 4) field RP_NOT_CENTERED_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    return sys_dsis_reg_TX_FIFO_1_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_1_INT_BIT_RP_NOT_CENTERED_1_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_1_INT_BIT_RP_NOT_CENTERED_1_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_1_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_1_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_1_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_1_I_set_to_clear", value );

    /* (0x000011fc bits 3) field RP_OUT_OF_SYNC_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    sys_dsis_reg_TX_FIFO_1_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_1_INT_BIT_RP_OUT_OF_SYNC_1_I_MSK,
                                                          DSIS_REG_TX_FIFO_1_INT_BIT_RP_OUT_OF_SYNC_1_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_1_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_1_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011fc bits 3) field RP_OUT_OF_SYNC_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    reg_value = sys_dsis_reg_TX_FIFO_1_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_1_INT_BIT_RP_OUT_OF_SYNC_1_I_MSK) >> DSIS_REG_TX_FIFO_1_INT_BIT_RP_OUT_OF_SYNC_1_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_1_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_1_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_1_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_1_I_poll", value );

    /* (0x000011fc bits 3) field RP_OUT_OF_SYNC_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    return sys_dsis_reg_TX_FIFO_1_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_1_INT_BIT_RP_OUT_OF_SYNC_1_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_1_INT_BIT_RP_OUT_OF_SYNC_1_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_1_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_1_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_1_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_1_I_set_to_clear", value );

    /* (0x000011fc bits 2) field RP_ABOVE_EXP_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    sys_dsis_reg_TX_FIFO_1_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_1_INT_BIT_RP_ABOVE_EXP_1_I_MSK,
                                                          DSIS_REG_TX_FIFO_1_INT_BIT_RP_ABOVE_EXP_1_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_1_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_1_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011fc bits 2) field RP_ABOVE_EXP_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    reg_value = sys_dsis_reg_TX_FIFO_1_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_1_INT_BIT_RP_ABOVE_EXP_1_I_MSK) >> DSIS_REG_TX_FIFO_1_INT_BIT_RP_ABOVE_EXP_1_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_1_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_1_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_1_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_1_I_poll", value );

    /* (0x000011fc bits 2) field RP_ABOVE_EXP_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    return sys_dsis_reg_TX_FIFO_1_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_1_INT_BIT_RP_ABOVE_EXP_1_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_1_INT_BIT_RP_ABOVE_EXP_1_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_1_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_1_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_1_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_1_I_set_to_clear", value );

    /* (0x000011fc bits 1) field RP_BELOW_EXP_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    sys_dsis_reg_TX_FIFO_1_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_1_INT_BIT_RP_BELOW_EXP_1_I_MSK,
                                                          DSIS_REG_TX_FIFO_1_INT_BIT_RP_BELOW_EXP_1_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_1_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_1_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011fc bits 1) field RP_BELOW_EXP_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    reg_value = sys_dsis_reg_TX_FIFO_1_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_1_INT_BIT_RP_BELOW_EXP_1_I_MSK) >> DSIS_REG_TX_FIFO_1_INT_BIT_RP_BELOW_EXP_1_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_1_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_1_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_1_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_1_I_poll", value );

    /* (0x000011fc bits 1) field RP_BELOW_EXP_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    return sys_dsis_reg_TX_FIFO_1_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_1_INT_BIT_RP_BELOW_EXP_1_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_1_INT_BIT_RP_BELOW_EXP_1_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_1_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_1_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_1_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_1_I_set_to_clear", value );

    /* (0x000011fc bits 0) field RP_IN_SYNC_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    sys_dsis_reg_TX_FIFO_1_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_1_INT_BIT_RP_IN_SYNC_1_I_MSK,
                                                          DSIS_REG_TX_FIFO_1_INT_BIT_RP_IN_SYNC_1_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_1_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_1_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000011fc bits 0) field RP_IN_SYNC_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    reg_value = sys_dsis_reg_TX_FIFO_1_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_1_INT_BIT_RP_IN_SYNC_1_I_MSK) >> DSIS_REG_TX_FIFO_1_INT_BIT_RP_IN_SYNC_1_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_1_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_1_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_1_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_1_I_poll", value );

    /* (0x000011fc bits 0) field RP_IN_SYNC_1_I of register PMC_DSIS_REG_TX_FIFO_1_INT */
    return sys_dsis_reg_TX_FIFO_1_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_1_INT_BIT_RP_IN_SYNC_1_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_1_INT_BIT_RP_IN_SYNC_1_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_NOT_CENTERED_2_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_2_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_2_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_2_I_set_to_clear", value );

    /* (0x00001200 bits 4) field RP_NOT_CENTERED_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    sys_dsis_reg_TX_FIFO_2_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_2_INT_BIT_RP_NOT_CENTERED_2_I_MSK,
                                                          DSIS_REG_TX_FIFO_2_INT_BIT_RP_NOT_CENTERED_2_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_2_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_2_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001200 bits 4) field RP_NOT_CENTERED_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    reg_value = sys_dsis_reg_TX_FIFO_2_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_2_INT_BIT_RP_NOT_CENTERED_2_I_MSK) >> DSIS_REG_TX_FIFO_2_INT_BIT_RP_NOT_CENTERED_2_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_2_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_2_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_2_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_2_I_poll", value );

    /* (0x00001200 bits 4) field RP_NOT_CENTERED_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    return sys_dsis_reg_TX_FIFO_2_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_2_INT_BIT_RP_NOT_CENTERED_2_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_2_INT_BIT_RP_NOT_CENTERED_2_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_2_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_2_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_2_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_2_I_set_to_clear", value );

    /* (0x00001200 bits 3) field RP_OUT_OF_SYNC_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    sys_dsis_reg_TX_FIFO_2_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_2_INT_BIT_RP_OUT_OF_SYNC_2_I_MSK,
                                                          DSIS_REG_TX_FIFO_2_INT_BIT_RP_OUT_OF_SYNC_2_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_2_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_2_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001200 bits 3) field RP_OUT_OF_SYNC_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    reg_value = sys_dsis_reg_TX_FIFO_2_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_2_INT_BIT_RP_OUT_OF_SYNC_2_I_MSK) >> DSIS_REG_TX_FIFO_2_INT_BIT_RP_OUT_OF_SYNC_2_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_2_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_2_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_2_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_2_I_poll", value );

    /* (0x00001200 bits 3) field RP_OUT_OF_SYNC_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    return sys_dsis_reg_TX_FIFO_2_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_2_INT_BIT_RP_OUT_OF_SYNC_2_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_2_INT_BIT_RP_OUT_OF_SYNC_2_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_2_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_2_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_2_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_2_I_set_to_clear", value );

    /* (0x00001200 bits 2) field RP_ABOVE_EXP_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    sys_dsis_reg_TX_FIFO_2_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_2_INT_BIT_RP_ABOVE_EXP_2_I_MSK,
                                                          DSIS_REG_TX_FIFO_2_INT_BIT_RP_ABOVE_EXP_2_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_2_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_2_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001200 bits 2) field RP_ABOVE_EXP_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    reg_value = sys_dsis_reg_TX_FIFO_2_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_2_INT_BIT_RP_ABOVE_EXP_2_I_MSK) >> DSIS_REG_TX_FIFO_2_INT_BIT_RP_ABOVE_EXP_2_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_2_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_2_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_2_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_2_I_poll", value );

    /* (0x00001200 bits 2) field RP_ABOVE_EXP_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    return sys_dsis_reg_TX_FIFO_2_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_2_INT_BIT_RP_ABOVE_EXP_2_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_2_INT_BIT_RP_ABOVE_EXP_2_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_2_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_2_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_2_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_2_I_set_to_clear", value );

    /* (0x00001200 bits 1) field RP_BELOW_EXP_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    sys_dsis_reg_TX_FIFO_2_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_2_INT_BIT_RP_BELOW_EXP_2_I_MSK,
                                                          DSIS_REG_TX_FIFO_2_INT_BIT_RP_BELOW_EXP_2_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_2_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_2_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001200 bits 1) field RP_BELOW_EXP_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    reg_value = sys_dsis_reg_TX_FIFO_2_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_2_INT_BIT_RP_BELOW_EXP_2_I_MSK) >> DSIS_REG_TX_FIFO_2_INT_BIT_RP_BELOW_EXP_2_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_2_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_2_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_2_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_2_I_poll", value );

    /* (0x00001200 bits 1) field RP_BELOW_EXP_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    return sys_dsis_reg_TX_FIFO_2_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_2_INT_BIT_RP_BELOW_EXP_2_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_2_INT_BIT_RP_BELOW_EXP_2_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_2_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_2_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_2_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_2_I_set_to_clear", value );

    /* (0x00001200 bits 0) field RP_IN_SYNC_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    sys_dsis_reg_TX_FIFO_2_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_2_INT_BIT_RP_IN_SYNC_2_I_MSK,
                                                          DSIS_REG_TX_FIFO_2_INT_BIT_RP_IN_SYNC_2_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_2_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_2_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001200 bits 0) field RP_IN_SYNC_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    reg_value = sys_dsis_reg_TX_FIFO_2_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_2_INT_BIT_RP_IN_SYNC_2_I_MSK) >> DSIS_REG_TX_FIFO_2_INT_BIT_RP_IN_SYNC_2_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_2_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_2_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_2_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_2_I_poll", value );

    /* (0x00001200 bits 0) field RP_IN_SYNC_2_I of register PMC_DSIS_REG_TX_FIFO_2_INT */
    return sys_dsis_reg_TX_FIFO_2_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_2_INT_BIT_RP_IN_SYNC_2_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_2_INT_BIT_RP_IN_SYNC_2_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_NOT_CENTERED_3_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_3_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_3_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_3_I_set_to_clear", value );

    /* (0x00001204 bits 4) field RP_NOT_CENTERED_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    sys_dsis_reg_TX_FIFO_3_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_3_INT_BIT_RP_NOT_CENTERED_3_I_MSK,
                                                          DSIS_REG_TX_FIFO_3_INT_BIT_RP_NOT_CENTERED_3_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_3_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_3_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001204 bits 4) field RP_NOT_CENTERED_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    reg_value = sys_dsis_reg_TX_FIFO_3_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_3_INT_BIT_RP_NOT_CENTERED_3_I_MSK) >> DSIS_REG_TX_FIFO_3_INT_BIT_RP_NOT_CENTERED_3_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_3_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_3_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_3_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_3_I_poll", value );

    /* (0x00001204 bits 4) field RP_NOT_CENTERED_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    return sys_dsis_reg_TX_FIFO_3_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_3_INT_BIT_RP_NOT_CENTERED_3_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_3_INT_BIT_RP_NOT_CENTERED_3_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_3_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_3_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_3_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_3_I_set_to_clear", value );

    /* (0x00001204 bits 3) field RP_OUT_OF_SYNC_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    sys_dsis_reg_TX_FIFO_3_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_3_INT_BIT_RP_OUT_OF_SYNC_3_I_MSK,
                                                          DSIS_REG_TX_FIFO_3_INT_BIT_RP_OUT_OF_SYNC_3_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_3_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_3_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001204 bits 3) field RP_OUT_OF_SYNC_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    reg_value = sys_dsis_reg_TX_FIFO_3_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_3_INT_BIT_RP_OUT_OF_SYNC_3_I_MSK) >> DSIS_REG_TX_FIFO_3_INT_BIT_RP_OUT_OF_SYNC_3_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_3_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_3_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_3_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_3_I_poll", value );

    /* (0x00001204 bits 3) field RP_OUT_OF_SYNC_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    return sys_dsis_reg_TX_FIFO_3_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_3_INT_BIT_RP_OUT_OF_SYNC_3_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_3_INT_BIT_RP_OUT_OF_SYNC_3_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_3_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_3_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_3_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_3_I_set_to_clear", value );

    /* (0x00001204 bits 2) field RP_ABOVE_EXP_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    sys_dsis_reg_TX_FIFO_3_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_3_INT_BIT_RP_ABOVE_EXP_3_I_MSK,
                                                          DSIS_REG_TX_FIFO_3_INT_BIT_RP_ABOVE_EXP_3_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_3_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_3_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001204 bits 2) field RP_ABOVE_EXP_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    reg_value = sys_dsis_reg_TX_FIFO_3_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_3_INT_BIT_RP_ABOVE_EXP_3_I_MSK) >> DSIS_REG_TX_FIFO_3_INT_BIT_RP_ABOVE_EXP_3_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_3_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_3_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_3_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_3_I_poll", value );

    /* (0x00001204 bits 2) field RP_ABOVE_EXP_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    return sys_dsis_reg_TX_FIFO_3_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_3_INT_BIT_RP_ABOVE_EXP_3_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_3_INT_BIT_RP_ABOVE_EXP_3_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_3_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_3_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_3_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_3_I_set_to_clear", value );

    /* (0x00001204 bits 1) field RP_BELOW_EXP_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    sys_dsis_reg_TX_FIFO_3_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_3_INT_BIT_RP_BELOW_EXP_3_I_MSK,
                                                          DSIS_REG_TX_FIFO_3_INT_BIT_RP_BELOW_EXP_3_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_3_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_3_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001204 bits 1) field RP_BELOW_EXP_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    reg_value = sys_dsis_reg_TX_FIFO_3_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_3_INT_BIT_RP_BELOW_EXP_3_I_MSK) >> DSIS_REG_TX_FIFO_3_INT_BIT_RP_BELOW_EXP_3_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_3_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_3_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_3_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_3_I_poll", value );

    /* (0x00001204 bits 1) field RP_BELOW_EXP_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    return sys_dsis_reg_TX_FIFO_3_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_3_INT_BIT_RP_BELOW_EXP_3_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_3_INT_BIT_RP_BELOW_EXP_3_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_3_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_3_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_3_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_3_I_set_to_clear", value );

    /* (0x00001204 bits 0) field RP_IN_SYNC_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    sys_dsis_reg_TX_FIFO_3_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_3_INT_BIT_RP_IN_SYNC_3_I_MSK,
                                                          DSIS_REG_TX_FIFO_3_INT_BIT_RP_IN_SYNC_3_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_3_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_3_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001204 bits 0) field RP_IN_SYNC_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    reg_value = sys_dsis_reg_TX_FIFO_3_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_3_INT_BIT_RP_IN_SYNC_3_I_MSK) >> DSIS_REG_TX_FIFO_3_INT_BIT_RP_IN_SYNC_3_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_3_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_3_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_3_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_3_I_poll", value );

    /* (0x00001204 bits 0) field RP_IN_SYNC_3_I of register PMC_DSIS_REG_TX_FIFO_3_INT */
    return sys_dsis_reg_TX_FIFO_3_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_3_INT_BIT_RP_IN_SYNC_3_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_3_INT_BIT_RP_IN_SYNC_3_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_NOT_CENTERED_4_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_4_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_4_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_4_I_set_to_clear", value );

    /* (0x00001208 bits 4) field RP_NOT_CENTERED_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    sys_dsis_reg_TX_FIFO_4_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_4_INT_BIT_RP_NOT_CENTERED_4_I_MSK,
                                                          DSIS_REG_TX_FIFO_4_INT_BIT_RP_NOT_CENTERED_4_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_4_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_4_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001208 bits 4) field RP_NOT_CENTERED_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    reg_value = sys_dsis_reg_TX_FIFO_4_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_4_INT_BIT_RP_NOT_CENTERED_4_I_MSK) >> DSIS_REG_TX_FIFO_4_INT_BIT_RP_NOT_CENTERED_4_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_4_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_4_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_4_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_4_I_poll", value );

    /* (0x00001208 bits 4) field RP_NOT_CENTERED_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    return sys_dsis_reg_TX_FIFO_4_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_4_INT_BIT_RP_NOT_CENTERED_4_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_4_INT_BIT_RP_NOT_CENTERED_4_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_4_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_4_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_4_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_4_I_set_to_clear", value );

    /* (0x00001208 bits 3) field RP_OUT_OF_SYNC_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    sys_dsis_reg_TX_FIFO_4_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_4_INT_BIT_RP_OUT_OF_SYNC_4_I_MSK,
                                                          DSIS_REG_TX_FIFO_4_INT_BIT_RP_OUT_OF_SYNC_4_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_4_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_4_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001208 bits 3) field RP_OUT_OF_SYNC_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    reg_value = sys_dsis_reg_TX_FIFO_4_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_4_INT_BIT_RP_OUT_OF_SYNC_4_I_MSK) >> DSIS_REG_TX_FIFO_4_INT_BIT_RP_OUT_OF_SYNC_4_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_4_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_4_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_4_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_4_I_poll", value );

    /* (0x00001208 bits 3) field RP_OUT_OF_SYNC_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    return sys_dsis_reg_TX_FIFO_4_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_4_INT_BIT_RP_OUT_OF_SYNC_4_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_4_INT_BIT_RP_OUT_OF_SYNC_4_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_4_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_4_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_4_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_4_I_set_to_clear", value );

    /* (0x00001208 bits 2) field RP_ABOVE_EXP_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    sys_dsis_reg_TX_FIFO_4_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_4_INT_BIT_RP_ABOVE_EXP_4_I_MSK,
                                                          DSIS_REG_TX_FIFO_4_INT_BIT_RP_ABOVE_EXP_4_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_4_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_4_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001208 bits 2) field RP_ABOVE_EXP_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    reg_value = sys_dsis_reg_TX_FIFO_4_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_4_INT_BIT_RP_ABOVE_EXP_4_I_MSK) >> DSIS_REG_TX_FIFO_4_INT_BIT_RP_ABOVE_EXP_4_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_4_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_4_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_4_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_4_I_poll", value );

    /* (0x00001208 bits 2) field RP_ABOVE_EXP_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    return sys_dsis_reg_TX_FIFO_4_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_4_INT_BIT_RP_ABOVE_EXP_4_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_4_INT_BIT_RP_ABOVE_EXP_4_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_4_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_4_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_4_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_4_I_set_to_clear", value );

    /* (0x00001208 bits 1) field RP_BELOW_EXP_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    sys_dsis_reg_TX_FIFO_4_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_4_INT_BIT_RP_BELOW_EXP_4_I_MSK,
                                                          DSIS_REG_TX_FIFO_4_INT_BIT_RP_BELOW_EXP_4_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_4_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_4_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001208 bits 1) field RP_BELOW_EXP_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    reg_value = sys_dsis_reg_TX_FIFO_4_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_4_INT_BIT_RP_BELOW_EXP_4_I_MSK) >> DSIS_REG_TX_FIFO_4_INT_BIT_RP_BELOW_EXP_4_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_4_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_4_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_4_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_4_I_poll", value );

    /* (0x00001208 bits 1) field RP_BELOW_EXP_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    return sys_dsis_reg_TX_FIFO_4_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_4_INT_BIT_RP_BELOW_EXP_4_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_4_INT_BIT_RP_BELOW_EXP_4_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_4_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_4_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_4_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_4_I_set_to_clear", value );

    /* (0x00001208 bits 0) field RP_IN_SYNC_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    sys_dsis_reg_TX_FIFO_4_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_4_INT_BIT_RP_IN_SYNC_4_I_MSK,
                                                          DSIS_REG_TX_FIFO_4_INT_BIT_RP_IN_SYNC_4_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_4_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_4_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001208 bits 0) field RP_IN_SYNC_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    reg_value = sys_dsis_reg_TX_FIFO_4_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_4_INT_BIT_RP_IN_SYNC_4_I_MSK) >> DSIS_REG_TX_FIFO_4_INT_BIT_RP_IN_SYNC_4_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_4_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_4_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_4_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_4_I_poll", value );

    /* (0x00001208 bits 0) field RP_IN_SYNC_4_I of register PMC_DSIS_REG_TX_FIFO_4_INT */
    return sys_dsis_reg_TX_FIFO_4_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_4_INT_BIT_RP_IN_SYNC_4_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_4_INT_BIT_RP_IN_SYNC_4_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_NOT_CENTERED_5_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_5_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_5_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_5_I_set_to_clear", value );

    /* (0x0000120c bits 4) field RP_NOT_CENTERED_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    sys_dsis_reg_TX_FIFO_5_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_5_INT_BIT_RP_NOT_CENTERED_5_I_MSK,
                                                          DSIS_REG_TX_FIFO_5_INT_BIT_RP_NOT_CENTERED_5_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_5_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_5_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000120c bits 4) field RP_NOT_CENTERED_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    reg_value = sys_dsis_reg_TX_FIFO_5_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_5_INT_BIT_RP_NOT_CENTERED_5_I_MSK) >> DSIS_REG_TX_FIFO_5_INT_BIT_RP_NOT_CENTERED_5_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_5_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_5_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_5_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_5_I_poll", value );

    /* (0x0000120c bits 4) field RP_NOT_CENTERED_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    return sys_dsis_reg_TX_FIFO_5_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_5_INT_BIT_RP_NOT_CENTERED_5_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_5_INT_BIT_RP_NOT_CENTERED_5_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_5_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_5_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_5_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_5_I_set_to_clear", value );

    /* (0x0000120c bits 3) field RP_OUT_OF_SYNC_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    sys_dsis_reg_TX_FIFO_5_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_5_INT_BIT_RP_OUT_OF_SYNC_5_I_MSK,
                                                          DSIS_REG_TX_FIFO_5_INT_BIT_RP_OUT_OF_SYNC_5_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_5_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_5_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000120c bits 3) field RP_OUT_OF_SYNC_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    reg_value = sys_dsis_reg_TX_FIFO_5_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_5_INT_BIT_RP_OUT_OF_SYNC_5_I_MSK) >> DSIS_REG_TX_FIFO_5_INT_BIT_RP_OUT_OF_SYNC_5_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_5_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_5_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_5_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_5_I_poll", value );

    /* (0x0000120c bits 3) field RP_OUT_OF_SYNC_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    return sys_dsis_reg_TX_FIFO_5_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_5_INT_BIT_RP_OUT_OF_SYNC_5_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_5_INT_BIT_RP_OUT_OF_SYNC_5_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_5_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_5_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_5_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_5_I_set_to_clear", value );

    /* (0x0000120c bits 2) field RP_ABOVE_EXP_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    sys_dsis_reg_TX_FIFO_5_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_5_INT_BIT_RP_ABOVE_EXP_5_I_MSK,
                                                          DSIS_REG_TX_FIFO_5_INT_BIT_RP_ABOVE_EXP_5_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_5_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_5_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000120c bits 2) field RP_ABOVE_EXP_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    reg_value = sys_dsis_reg_TX_FIFO_5_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_5_INT_BIT_RP_ABOVE_EXP_5_I_MSK) >> DSIS_REG_TX_FIFO_5_INT_BIT_RP_ABOVE_EXP_5_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_5_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_5_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_5_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_5_I_poll", value );

    /* (0x0000120c bits 2) field RP_ABOVE_EXP_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    return sys_dsis_reg_TX_FIFO_5_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_5_INT_BIT_RP_ABOVE_EXP_5_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_5_INT_BIT_RP_ABOVE_EXP_5_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_5_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_5_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_5_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_5_I_set_to_clear", value );

    /* (0x0000120c bits 1) field RP_BELOW_EXP_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    sys_dsis_reg_TX_FIFO_5_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_5_INT_BIT_RP_BELOW_EXP_5_I_MSK,
                                                          DSIS_REG_TX_FIFO_5_INT_BIT_RP_BELOW_EXP_5_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_5_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_5_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000120c bits 1) field RP_BELOW_EXP_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    reg_value = sys_dsis_reg_TX_FIFO_5_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_5_INT_BIT_RP_BELOW_EXP_5_I_MSK) >> DSIS_REG_TX_FIFO_5_INT_BIT_RP_BELOW_EXP_5_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_5_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_5_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_5_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_5_I_poll", value );

    /* (0x0000120c bits 1) field RP_BELOW_EXP_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    return sys_dsis_reg_TX_FIFO_5_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_5_INT_BIT_RP_BELOW_EXP_5_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_5_INT_BIT_RP_BELOW_EXP_5_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_5_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_5_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_5_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_5_I_set_to_clear", value );

    /* (0x0000120c bits 0) field RP_IN_SYNC_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    sys_dsis_reg_TX_FIFO_5_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_5_INT_BIT_RP_IN_SYNC_5_I_MSK,
                                                          DSIS_REG_TX_FIFO_5_INT_BIT_RP_IN_SYNC_5_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_5_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_5_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000120c bits 0) field RP_IN_SYNC_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    reg_value = sys_dsis_reg_TX_FIFO_5_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_5_INT_BIT_RP_IN_SYNC_5_I_MSK) >> DSIS_REG_TX_FIFO_5_INT_BIT_RP_IN_SYNC_5_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_5_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_5_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_5_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_5_I_poll", value );

    /* (0x0000120c bits 0) field RP_IN_SYNC_5_I of register PMC_DSIS_REG_TX_FIFO_5_INT */
    return sys_dsis_reg_TX_FIFO_5_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_5_INT_BIT_RP_IN_SYNC_5_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_5_INT_BIT_RP_IN_SYNC_5_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_NOT_CENTERED_6_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_6_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_6_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_6_I_set_to_clear", value );

    /* (0x00001210 bits 4) field RP_NOT_CENTERED_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    sys_dsis_reg_TX_FIFO_6_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_6_INT_BIT_RP_NOT_CENTERED_6_I_MSK,
                                                          DSIS_REG_TX_FIFO_6_INT_BIT_RP_NOT_CENTERED_6_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_6_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_6_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001210 bits 4) field RP_NOT_CENTERED_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    reg_value = sys_dsis_reg_TX_FIFO_6_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_6_INT_BIT_RP_NOT_CENTERED_6_I_MSK) >> DSIS_REG_TX_FIFO_6_INT_BIT_RP_NOT_CENTERED_6_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_6_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_6_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_6_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_6_I_poll", value );

    /* (0x00001210 bits 4) field RP_NOT_CENTERED_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    return sys_dsis_reg_TX_FIFO_6_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_6_INT_BIT_RP_NOT_CENTERED_6_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_6_INT_BIT_RP_NOT_CENTERED_6_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_6_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_6_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_6_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_6_I_set_to_clear", value );

    /* (0x00001210 bits 3) field RP_OUT_OF_SYNC_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    sys_dsis_reg_TX_FIFO_6_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_6_INT_BIT_RP_OUT_OF_SYNC_6_I_MSK,
                                                          DSIS_REG_TX_FIFO_6_INT_BIT_RP_OUT_OF_SYNC_6_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_6_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_6_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001210 bits 3) field RP_OUT_OF_SYNC_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    reg_value = sys_dsis_reg_TX_FIFO_6_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_6_INT_BIT_RP_OUT_OF_SYNC_6_I_MSK) >> DSIS_REG_TX_FIFO_6_INT_BIT_RP_OUT_OF_SYNC_6_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_6_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_6_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_6_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_6_I_poll", value );

    /* (0x00001210 bits 3) field RP_OUT_OF_SYNC_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    return sys_dsis_reg_TX_FIFO_6_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_6_INT_BIT_RP_OUT_OF_SYNC_6_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_6_INT_BIT_RP_OUT_OF_SYNC_6_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_6_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_6_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_6_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_6_I_set_to_clear", value );

    /* (0x00001210 bits 2) field RP_ABOVE_EXP_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    sys_dsis_reg_TX_FIFO_6_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_6_INT_BIT_RP_ABOVE_EXP_6_I_MSK,
                                                          DSIS_REG_TX_FIFO_6_INT_BIT_RP_ABOVE_EXP_6_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_6_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_6_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001210 bits 2) field RP_ABOVE_EXP_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    reg_value = sys_dsis_reg_TX_FIFO_6_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_6_INT_BIT_RP_ABOVE_EXP_6_I_MSK) >> DSIS_REG_TX_FIFO_6_INT_BIT_RP_ABOVE_EXP_6_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_6_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_6_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_6_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_6_I_poll", value );

    /* (0x00001210 bits 2) field RP_ABOVE_EXP_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    return sys_dsis_reg_TX_FIFO_6_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_6_INT_BIT_RP_ABOVE_EXP_6_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_6_INT_BIT_RP_ABOVE_EXP_6_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_6_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_6_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_6_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_6_I_set_to_clear", value );

    /* (0x00001210 bits 1) field RP_BELOW_EXP_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    sys_dsis_reg_TX_FIFO_6_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_6_INT_BIT_RP_BELOW_EXP_6_I_MSK,
                                                          DSIS_REG_TX_FIFO_6_INT_BIT_RP_BELOW_EXP_6_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_6_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_6_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001210 bits 1) field RP_BELOW_EXP_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    reg_value = sys_dsis_reg_TX_FIFO_6_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_6_INT_BIT_RP_BELOW_EXP_6_I_MSK) >> DSIS_REG_TX_FIFO_6_INT_BIT_RP_BELOW_EXP_6_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_6_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_6_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_6_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_6_I_poll", value );

    /* (0x00001210 bits 1) field RP_BELOW_EXP_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    return sys_dsis_reg_TX_FIFO_6_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_6_INT_BIT_RP_BELOW_EXP_6_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_6_INT_BIT_RP_BELOW_EXP_6_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_6_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_6_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_6_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_6_I_set_to_clear", value );

    /* (0x00001210 bits 0) field RP_IN_SYNC_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    sys_dsis_reg_TX_FIFO_6_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_6_INT_BIT_RP_IN_SYNC_6_I_MSK,
                                                          DSIS_REG_TX_FIFO_6_INT_BIT_RP_IN_SYNC_6_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_6_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_6_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001210 bits 0) field RP_IN_SYNC_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    reg_value = sys_dsis_reg_TX_FIFO_6_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_6_INT_BIT_RP_IN_SYNC_6_I_MSK) >> DSIS_REG_TX_FIFO_6_INT_BIT_RP_IN_SYNC_6_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_6_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_6_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_6_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_6_I_poll", value );

    /* (0x00001210 bits 0) field RP_IN_SYNC_6_I of register PMC_DSIS_REG_TX_FIFO_6_INT */
    return sys_dsis_reg_TX_FIFO_6_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_6_INT_BIT_RP_IN_SYNC_6_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_6_INT_BIT_RP_IN_SYNC_6_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_NOT_CENTERED_7_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_7_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_7_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_7_I_set_to_clear", value );

    /* (0x00001214 bits 4) field RP_NOT_CENTERED_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    sys_dsis_reg_TX_FIFO_7_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_7_INT_BIT_RP_NOT_CENTERED_7_I_MSK,
                                                          DSIS_REG_TX_FIFO_7_INT_BIT_RP_NOT_CENTERED_7_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_7_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_7_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001214 bits 4) field RP_NOT_CENTERED_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    reg_value = sys_dsis_reg_TX_FIFO_7_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_7_INT_BIT_RP_NOT_CENTERED_7_I_MSK) >> DSIS_REG_TX_FIFO_7_INT_BIT_RP_NOT_CENTERED_7_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_7_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_7_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_7_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_7_I_poll", value );

    /* (0x00001214 bits 4) field RP_NOT_CENTERED_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    return sys_dsis_reg_TX_FIFO_7_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_7_INT_BIT_RP_NOT_CENTERED_7_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_7_INT_BIT_RP_NOT_CENTERED_7_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_7_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_7_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_7_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_7_I_set_to_clear", value );

    /* (0x00001214 bits 3) field RP_OUT_OF_SYNC_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    sys_dsis_reg_TX_FIFO_7_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_7_INT_BIT_RP_OUT_OF_SYNC_7_I_MSK,
                                                          DSIS_REG_TX_FIFO_7_INT_BIT_RP_OUT_OF_SYNC_7_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_7_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_7_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001214 bits 3) field RP_OUT_OF_SYNC_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    reg_value = sys_dsis_reg_TX_FIFO_7_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_7_INT_BIT_RP_OUT_OF_SYNC_7_I_MSK) >> DSIS_REG_TX_FIFO_7_INT_BIT_RP_OUT_OF_SYNC_7_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_7_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_7_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_7_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_7_I_poll", value );

    /* (0x00001214 bits 3) field RP_OUT_OF_SYNC_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    return sys_dsis_reg_TX_FIFO_7_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_7_INT_BIT_RP_OUT_OF_SYNC_7_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_7_INT_BIT_RP_OUT_OF_SYNC_7_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_7_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_7_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_7_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_7_I_set_to_clear", value );

    /* (0x00001214 bits 2) field RP_ABOVE_EXP_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    sys_dsis_reg_TX_FIFO_7_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_7_INT_BIT_RP_ABOVE_EXP_7_I_MSK,
                                                          DSIS_REG_TX_FIFO_7_INT_BIT_RP_ABOVE_EXP_7_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_7_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_7_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001214 bits 2) field RP_ABOVE_EXP_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    reg_value = sys_dsis_reg_TX_FIFO_7_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_7_INT_BIT_RP_ABOVE_EXP_7_I_MSK) >> DSIS_REG_TX_FIFO_7_INT_BIT_RP_ABOVE_EXP_7_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_7_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_7_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_7_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_7_I_poll", value );

    /* (0x00001214 bits 2) field RP_ABOVE_EXP_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    return sys_dsis_reg_TX_FIFO_7_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_7_INT_BIT_RP_ABOVE_EXP_7_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_7_INT_BIT_RP_ABOVE_EXP_7_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_7_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_7_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_7_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_7_I_set_to_clear", value );

    /* (0x00001214 bits 1) field RP_BELOW_EXP_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    sys_dsis_reg_TX_FIFO_7_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_7_INT_BIT_RP_BELOW_EXP_7_I_MSK,
                                                          DSIS_REG_TX_FIFO_7_INT_BIT_RP_BELOW_EXP_7_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_7_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_7_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001214 bits 1) field RP_BELOW_EXP_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    reg_value = sys_dsis_reg_TX_FIFO_7_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_7_INT_BIT_RP_BELOW_EXP_7_I_MSK) >> DSIS_REG_TX_FIFO_7_INT_BIT_RP_BELOW_EXP_7_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_7_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_7_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_7_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_7_I_poll", value );

    /* (0x00001214 bits 1) field RP_BELOW_EXP_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    return sys_dsis_reg_TX_FIFO_7_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_7_INT_BIT_RP_BELOW_EXP_7_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_7_INT_BIT_RP_BELOW_EXP_7_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_7_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_7_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_7_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_7_I_set_to_clear", value );

    /* (0x00001214 bits 0) field RP_IN_SYNC_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    sys_dsis_reg_TX_FIFO_7_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_7_INT_BIT_RP_IN_SYNC_7_I_MSK,
                                                          DSIS_REG_TX_FIFO_7_INT_BIT_RP_IN_SYNC_7_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_7_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_7_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001214 bits 0) field RP_IN_SYNC_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    reg_value = sys_dsis_reg_TX_FIFO_7_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_7_INT_BIT_RP_IN_SYNC_7_I_MSK) >> DSIS_REG_TX_FIFO_7_INT_BIT_RP_IN_SYNC_7_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_7_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_7_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_7_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_7_I_poll", value );

    /* (0x00001214 bits 0) field RP_IN_SYNC_7_I of register PMC_DSIS_REG_TX_FIFO_7_INT */
    return sys_dsis_reg_TX_FIFO_7_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_7_INT_BIT_RP_IN_SYNC_7_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_7_INT_BIT_RP_IN_SYNC_7_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_NOT_CENTERED_8_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_8_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_8_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_8_I_set_to_clear", value );

    /* (0x00001218 bits 4) field RP_NOT_CENTERED_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    sys_dsis_reg_TX_FIFO_8_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_8_INT_BIT_RP_NOT_CENTERED_8_I_MSK,
                                                          DSIS_REG_TX_FIFO_8_INT_BIT_RP_NOT_CENTERED_8_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_8_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_8_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001218 bits 4) field RP_NOT_CENTERED_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    reg_value = sys_dsis_reg_TX_FIFO_8_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_8_INT_BIT_RP_NOT_CENTERED_8_I_MSK) >> DSIS_REG_TX_FIFO_8_INT_BIT_RP_NOT_CENTERED_8_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_8_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_8_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_8_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_8_I_poll", value );

    /* (0x00001218 bits 4) field RP_NOT_CENTERED_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    return sys_dsis_reg_TX_FIFO_8_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_8_INT_BIT_RP_NOT_CENTERED_8_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_8_INT_BIT_RP_NOT_CENTERED_8_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_8_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_8_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_8_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_8_I_set_to_clear", value );

    /* (0x00001218 bits 3) field RP_OUT_OF_SYNC_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    sys_dsis_reg_TX_FIFO_8_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_8_INT_BIT_RP_OUT_OF_SYNC_8_I_MSK,
                                                          DSIS_REG_TX_FIFO_8_INT_BIT_RP_OUT_OF_SYNC_8_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_8_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_8_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001218 bits 3) field RP_OUT_OF_SYNC_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    reg_value = sys_dsis_reg_TX_FIFO_8_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_8_INT_BIT_RP_OUT_OF_SYNC_8_I_MSK) >> DSIS_REG_TX_FIFO_8_INT_BIT_RP_OUT_OF_SYNC_8_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_8_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_8_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_8_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_8_I_poll", value );

    /* (0x00001218 bits 3) field RP_OUT_OF_SYNC_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    return sys_dsis_reg_TX_FIFO_8_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_8_INT_BIT_RP_OUT_OF_SYNC_8_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_8_INT_BIT_RP_OUT_OF_SYNC_8_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_8_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_8_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_8_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_8_I_set_to_clear", value );

    /* (0x00001218 bits 2) field RP_ABOVE_EXP_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    sys_dsis_reg_TX_FIFO_8_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_8_INT_BIT_RP_ABOVE_EXP_8_I_MSK,
                                                          DSIS_REG_TX_FIFO_8_INT_BIT_RP_ABOVE_EXP_8_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_8_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_8_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001218 bits 2) field RP_ABOVE_EXP_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    reg_value = sys_dsis_reg_TX_FIFO_8_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_8_INT_BIT_RP_ABOVE_EXP_8_I_MSK) >> DSIS_REG_TX_FIFO_8_INT_BIT_RP_ABOVE_EXP_8_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_8_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_8_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_8_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_8_I_poll", value );

    /* (0x00001218 bits 2) field RP_ABOVE_EXP_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    return sys_dsis_reg_TX_FIFO_8_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_8_INT_BIT_RP_ABOVE_EXP_8_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_8_INT_BIT_RP_ABOVE_EXP_8_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_8_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_8_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_8_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_8_I_set_to_clear", value );

    /* (0x00001218 bits 1) field RP_BELOW_EXP_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    sys_dsis_reg_TX_FIFO_8_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_8_INT_BIT_RP_BELOW_EXP_8_I_MSK,
                                                          DSIS_REG_TX_FIFO_8_INT_BIT_RP_BELOW_EXP_8_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_8_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_8_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001218 bits 1) field RP_BELOW_EXP_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    reg_value = sys_dsis_reg_TX_FIFO_8_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_8_INT_BIT_RP_BELOW_EXP_8_I_MSK) >> DSIS_REG_TX_FIFO_8_INT_BIT_RP_BELOW_EXP_8_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_8_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_8_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_8_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_8_I_poll", value );

    /* (0x00001218 bits 1) field RP_BELOW_EXP_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    return sys_dsis_reg_TX_FIFO_8_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_8_INT_BIT_RP_BELOW_EXP_8_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_8_INT_BIT_RP_BELOW_EXP_8_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_8_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_8_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_8_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_8_I_set_to_clear", value );

    /* (0x00001218 bits 0) field RP_IN_SYNC_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    sys_dsis_reg_TX_FIFO_8_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_8_INT_BIT_RP_IN_SYNC_8_I_MSK,
                                                          DSIS_REG_TX_FIFO_8_INT_BIT_RP_IN_SYNC_8_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_8_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_8_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001218 bits 0) field RP_IN_SYNC_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    reg_value = sys_dsis_reg_TX_FIFO_8_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_8_INT_BIT_RP_IN_SYNC_8_I_MSK) >> DSIS_REG_TX_FIFO_8_INT_BIT_RP_IN_SYNC_8_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_8_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_8_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_8_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_8_I_poll", value );

    /* (0x00001218 bits 0) field RP_IN_SYNC_8_I of register PMC_DSIS_REG_TX_FIFO_8_INT */
    return sys_dsis_reg_TX_FIFO_8_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_8_INT_BIT_RP_IN_SYNC_8_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_8_INT_BIT_RP_IN_SYNC_8_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_NOT_CENTERED_9_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_9_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_9_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_9_I_set_to_clear", value );

    /* (0x0000121c bits 4) field RP_NOT_CENTERED_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    sys_dsis_reg_TX_FIFO_9_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_9_INT_BIT_RP_NOT_CENTERED_9_I_MSK,
                                                          DSIS_REG_TX_FIFO_9_INT_BIT_RP_NOT_CENTERED_9_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_9_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_9_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000121c bits 4) field RP_NOT_CENTERED_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    reg_value = sys_dsis_reg_TX_FIFO_9_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_9_INT_BIT_RP_NOT_CENTERED_9_I_MSK) >> DSIS_REG_TX_FIFO_9_INT_BIT_RP_NOT_CENTERED_9_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_9_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_9_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_9_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_9_I_poll", value );

    /* (0x0000121c bits 4) field RP_NOT_CENTERED_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    return sys_dsis_reg_TX_FIFO_9_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_9_INT_BIT_RP_NOT_CENTERED_9_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_9_INT_BIT_RP_NOT_CENTERED_9_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_9_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_9_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_9_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_9_I_set_to_clear", value );

    /* (0x0000121c bits 3) field RP_OUT_OF_SYNC_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    sys_dsis_reg_TX_FIFO_9_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_9_INT_BIT_RP_OUT_OF_SYNC_9_I_MSK,
                                                          DSIS_REG_TX_FIFO_9_INT_BIT_RP_OUT_OF_SYNC_9_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_9_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_9_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000121c bits 3) field RP_OUT_OF_SYNC_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    reg_value = sys_dsis_reg_TX_FIFO_9_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_9_INT_BIT_RP_OUT_OF_SYNC_9_I_MSK) >> DSIS_REG_TX_FIFO_9_INT_BIT_RP_OUT_OF_SYNC_9_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_9_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_9_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_9_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                           sifd_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_9_I_poll", value );

    /* (0x0000121c bits 3) field RP_OUT_OF_SYNC_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    return sys_dsis_reg_TX_FIFO_9_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_9_INT_BIT_RP_OUT_OF_SYNC_9_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_9_INT_BIT_RP_OUT_OF_SYNC_9_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_9_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_9_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_9_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_9_I_set_to_clear", value );

    /* (0x0000121c bits 2) field RP_ABOVE_EXP_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    sys_dsis_reg_TX_FIFO_9_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_9_INT_BIT_RP_ABOVE_EXP_9_I_MSK,
                                                          DSIS_REG_TX_FIFO_9_INT_BIT_RP_ABOVE_EXP_9_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_9_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_9_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000121c bits 2) field RP_ABOVE_EXP_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    reg_value = sys_dsis_reg_TX_FIFO_9_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_9_INT_BIT_RP_ABOVE_EXP_9_I_MSK) >> DSIS_REG_TX_FIFO_9_INT_BIT_RP_ABOVE_EXP_9_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_9_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_9_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_9_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_9_I_poll", value );

    /* (0x0000121c bits 2) field RP_ABOVE_EXP_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    return sys_dsis_reg_TX_FIFO_9_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_9_INT_BIT_RP_ABOVE_EXP_9_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_9_INT_BIT_RP_ABOVE_EXP_9_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_9_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_9_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_9_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_9_I_set_to_clear", value );

    /* (0x0000121c bits 1) field RP_BELOW_EXP_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    sys_dsis_reg_TX_FIFO_9_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_9_INT_BIT_RP_BELOW_EXP_9_I_MSK,
                                                          DSIS_REG_TX_FIFO_9_INT_BIT_RP_BELOW_EXP_9_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_9_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_9_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000121c bits 1) field RP_BELOW_EXP_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    reg_value = sys_dsis_reg_TX_FIFO_9_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_9_INT_BIT_RP_BELOW_EXP_9_I_MSK) >> DSIS_REG_TX_FIFO_9_INT_BIT_RP_BELOW_EXP_9_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_9_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_9_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_9_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_9_I_poll", value );

    /* (0x0000121c bits 1) field RP_BELOW_EXP_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    return sys_dsis_reg_TX_FIFO_9_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_9_INT_BIT_RP_BELOW_EXP_9_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_9_INT_BIT_RP_BELOW_EXP_9_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_9_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_9_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_9_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_9_I_set_to_clear", value );

    /* (0x0000121c bits 0) field RP_IN_SYNC_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    sys_dsis_reg_TX_FIFO_9_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          DSIS_REG_TX_FIFO_9_INT_BIT_RP_IN_SYNC_9_I_MSK,
                                                          DSIS_REG_TX_FIFO_9_INT_BIT_RP_IN_SYNC_9_I_OFF,
                                                          value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_9_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_9_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000121c bits 0) field RP_IN_SYNC_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    reg_value = sys_dsis_reg_TX_FIFO_9_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_9_INT_BIT_RP_IN_SYNC_9_I_MSK) >> DSIS_REG_TX_FIFO_9_INT_BIT_RP_IN_SYNC_9_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_9_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_9_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_9_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_9_I_poll", value );

    /* (0x0000121c bits 0) field RP_IN_SYNC_9_I of register PMC_DSIS_REG_TX_FIFO_9_INT */
    return sys_dsis_reg_TX_FIFO_9_INT_poll( b_ptr,
                                            h_ptr,
                                            DSIS_REG_TX_FIFO_9_INT_BIT_RP_IN_SYNC_9_I_MSK,
                                            (value<<DSIS_REG_TX_FIFO_9_INT_BIT_RP_IN_SYNC_9_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_NOT_CENTERED_10_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_10_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_10_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_10_I_set_to_clear", value );

    /* (0x00001220 bits 4) field RP_NOT_CENTERED_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    sys_dsis_reg_TX_FIFO_10_INT_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           DSIS_REG_TX_FIFO_10_INT_BIT_RP_NOT_CENTERED_10_I_MSK,
                                                           DSIS_REG_TX_FIFO_10_INT_BIT_RP_NOT_CENTERED_10_I_OFF,
                                                           value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_10_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_10_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001220 bits 4) field RP_NOT_CENTERED_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    reg_value = sys_dsis_reg_TX_FIFO_10_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_10_INT_BIT_RP_NOT_CENTERED_10_I_MSK) >> DSIS_REG_TX_FIFO_10_INT_BIT_RP_NOT_CENTERED_10_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_10_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_10_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_10_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_10_I_poll", value );

    /* (0x00001220 bits 4) field RP_NOT_CENTERED_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    return sys_dsis_reg_TX_FIFO_10_INT_poll( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_10_INT_BIT_RP_NOT_CENTERED_10_I_MSK,
                                             (value<<DSIS_REG_TX_FIFO_10_INT_BIT_RP_NOT_CENTERED_10_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_10_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_10_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_10_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_10_I_set_to_clear", value );

    /* (0x00001220 bits 3) field RP_OUT_OF_SYNC_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    sys_dsis_reg_TX_FIFO_10_INT_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           DSIS_REG_TX_FIFO_10_INT_BIT_RP_OUT_OF_SYNC_10_I_MSK,
                                                           DSIS_REG_TX_FIFO_10_INT_BIT_RP_OUT_OF_SYNC_10_I_OFF,
                                                           value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_10_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_10_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001220 bits 3) field RP_OUT_OF_SYNC_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    reg_value = sys_dsis_reg_TX_FIFO_10_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_10_INT_BIT_RP_OUT_OF_SYNC_10_I_MSK) >> DSIS_REG_TX_FIFO_10_INT_BIT_RP_OUT_OF_SYNC_10_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_10_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_10_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_10_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_10_I_poll", value );

    /* (0x00001220 bits 3) field RP_OUT_OF_SYNC_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    return sys_dsis_reg_TX_FIFO_10_INT_poll( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_10_INT_BIT_RP_OUT_OF_SYNC_10_I_MSK,
                                             (value<<DSIS_REG_TX_FIFO_10_INT_BIT_RP_OUT_OF_SYNC_10_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_10_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_10_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_10_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_10_I_set_to_clear", value );

    /* (0x00001220 bits 2) field RP_ABOVE_EXP_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    sys_dsis_reg_TX_FIFO_10_INT_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           DSIS_REG_TX_FIFO_10_INT_BIT_RP_ABOVE_EXP_10_I_MSK,
                                                           DSIS_REG_TX_FIFO_10_INT_BIT_RP_ABOVE_EXP_10_I_OFF,
                                                           value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_10_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_10_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001220 bits 2) field RP_ABOVE_EXP_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    reg_value = sys_dsis_reg_TX_FIFO_10_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_10_INT_BIT_RP_ABOVE_EXP_10_I_MSK) >> DSIS_REG_TX_FIFO_10_INT_BIT_RP_ABOVE_EXP_10_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_10_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_10_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_10_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_10_I_poll", value );

    /* (0x00001220 bits 2) field RP_ABOVE_EXP_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    return sys_dsis_reg_TX_FIFO_10_INT_poll( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_10_INT_BIT_RP_ABOVE_EXP_10_I_MSK,
                                             (value<<DSIS_REG_TX_FIFO_10_INT_BIT_RP_ABOVE_EXP_10_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_10_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_10_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_10_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_10_I_set_to_clear", value );

    /* (0x00001220 bits 1) field RP_BELOW_EXP_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    sys_dsis_reg_TX_FIFO_10_INT_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           DSIS_REG_TX_FIFO_10_INT_BIT_RP_BELOW_EXP_10_I_MSK,
                                                           DSIS_REG_TX_FIFO_10_INT_BIT_RP_BELOW_EXP_10_I_OFF,
                                                           value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_10_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_10_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001220 bits 1) field RP_BELOW_EXP_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    reg_value = sys_dsis_reg_TX_FIFO_10_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_10_INT_BIT_RP_BELOW_EXP_10_I_MSK) >> DSIS_REG_TX_FIFO_10_INT_BIT_RP_BELOW_EXP_10_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_10_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_10_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_10_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_10_I_poll", value );

    /* (0x00001220 bits 1) field RP_BELOW_EXP_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    return sys_dsis_reg_TX_FIFO_10_INT_poll( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_10_INT_BIT_RP_BELOW_EXP_10_I_MSK,
                                             (value<<DSIS_REG_TX_FIFO_10_INT_BIT_RP_BELOW_EXP_10_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_10_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_10_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_10_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_10_I_set_to_clear", value );

    /* (0x00001220 bits 0) field RP_IN_SYNC_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    sys_dsis_reg_TX_FIFO_10_INT_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           DSIS_REG_TX_FIFO_10_INT_BIT_RP_IN_SYNC_10_I_MSK,
                                                           DSIS_REG_TX_FIFO_10_INT_BIT_RP_IN_SYNC_10_I_OFF,
                                                           value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_10_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_10_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001220 bits 0) field RP_IN_SYNC_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    reg_value = sys_dsis_reg_TX_FIFO_10_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_10_INT_BIT_RP_IN_SYNC_10_I_MSK) >> DSIS_REG_TX_FIFO_10_INT_BIT_RP_IN_SYNC_10_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_10_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_10_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_10_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_10_I_poll", value );

    /* (0x00001220 bits 0) field RP_IN_SYNC_10_I of register PMC_DSIS_REG_TX_FIFO_10_INT */
    return sys_dsis_reg_TX_FIFO_10_INT_poll( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_10_INT_BIT_RP_IN_SYNC_10_I_MSK,
                                             (value<<DSIS_REG_TX_FIFO_10_INT_BIT_RP_IN_SYNC_10_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_NOT_CENTERED_11_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_NOT_CENTERED_11_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_11_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_11_I_set_to_clear", value );

    /* (0x00001224 bits 4) field RP_NOT_CENTERED_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    sys_dsis_reg_TX_FIFO_11_INT_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           DSIS_REG_TX_FIFO_11_INT_BIT_RP_NOT_CENTERED_11_I_MSK,
                                                           DSIS_REG_TX_FIFO_11_INT_BIT_RP_NOT_CENTERED_11_I_OFF,
                                                           value);
}

static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_11_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_NOT_CENTERED_11_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001224 bits 4) field RP_NOT_CENTERED_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    reg_value = sys_dsis_reg_TX_FIFO_11_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_11_INT_BIT_RP_NOT_CENTERED_11_I_MSK) >> DSIS_REG_TX_FIFO_11_INT_BIT_RP_NOT_CENTERED_11_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_NOT_CENTERED_11_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_11_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_11_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_NOT_CENTERED_11_I_poll", value );

    /* (0x00001224 bits 4) field RP_NOT_CENTERED_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    return sys_dsis_reg_TX_FIFO_11_INT_poll( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_11_INT_BIT_RP_NOT_CENTERED_11_I_MSK,
                                             (value<<DSIS_REG_TX_FIFO_11_INT_BIT_RP_NOT_CENTERED_11_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_11_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_OUT_OF_SYNC_11_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_11_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_11_I_set_to_clear", value );

    /* (0x00001224 bits 3) field RP_OUT_OF_SYNC_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    sys_dsis_reg_TX_FIFO_11_INT_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           DSIS_REG_TX_FIFO_11_INT_BIT_RP_OUT_OF_SYNC_11_I_MSK,
                                                           DSIS_REG_TX_FIFO_11_INT_BIT_RP_OUT_OF_SYNC_11_I_OFF,
                                                           value);
}

static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_11_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_OUT_OF_SYNC_11_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001224 bits 3) field RP_OUT_OF_SYNC_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    reg_value = sys_dsis_reg_TX_FIFO_11_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_11_INT_BIT_RP_OUT_OF_SYNC_11_I_MSK) >> DSIS_REG_TX_FIFO_11_INT_BIT_RP_OUT_OF_SYNC_11_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_11_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_11_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_11_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                            sifd_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_OUT_OF_SYNC_11_I_poll", value );

    /* (0x00001224 bits 3) field RP_OUT_OF_SYNC_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    return sys_dsis_reg_TX_FIFO_11_INT_poll( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_11_INT_BIT_RP_OUT_OF_SYNC_11_I_MSK,
                                             (value<<DSIS_REG_TX_FIFO_11_INT_BIT_RP_OUT_OF_SYNC_11_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_ABOVE_EXP_11_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_ABOVE_EXP_11_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_11_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_11_I_set_to_clear", value );

    /* (0x00001224 bits 2) field RP_ABOVE_EXP_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    sys_dsis_reg_TX_FIFO_11_INT_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           DSIS_REG_TX_FIFO_11_INT_BIT_RP_ABOVE_EXP_11_I_MSK,
                                                           DSIS_REG_TX_FIFO_11_INT_BIT_RP_ABOVE_EXP_11_I_OFF,
                                                           value);
}

static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_11_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_ABOVE_EXP_11_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001224 bits 2) field RP_ABOVE_EXP_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    reg_value = sys_dsis_reg_TX_FIFO_11_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_11_INT_BIT_RP_ABOVE_EXP_11_I_MSK) >> DSIS_REG_TX_FIFO_11_INT_BIT_RP_ABOVE_EXP_11_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_ABOVE_EXP_11_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_11_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_11_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_ABOVE_EXP_11_I_poll", value );

    /* (0x00001224 bits 2) field RP_ABOVE_EXP_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    return sys_dsis_reg_TX_FIFO_11_INT_poll( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_11_INT_BIT_RP_ABOVE_EXP_11_I_MSK,
                                             (value<<DSIS_REG_TX_FIFO_11_INT_BIT_RP_ABOVE_EXP_11_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_BELOW_EXP_11_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_BELOW_EXP_11_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_BELOW_EXP_11_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_BELOW_EXP_11_I_set_to_clear", value );

    /* (0x00001224 bits 1) field RP_BELOW_EXP_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    sys_dsis_reg_TX_FIFO_11_INT_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           DSIS_REG_TX_FIFO_11_INT_BIT_RP_BELOW_EXP_11_I_MSK,
                                                           DSIS_REG_TX_FIFO_11_INT_BIT_RP_BELOW_EXP_11_I_OFF,
                                                           value);
}

static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_11_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_BELOW_EXP_11_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001224 bits 1) field RP_BELOW_EXP_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    reg_value = sys_dsis_reg_TX_FIFO_11_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_11_INT_BIT_RP_BELOW_EXP_11_I_MSK) >> DSIS_REG_TX_FIFO_11_INT_BIT_RP_BELOW_EXP_11_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_BELOW_EXP_11_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_11_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_11_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_BELOW_EXP_11_I_poll", value );

    /* (0x00001224 bits 1) field RP_BELOW_EXP_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    return sys_dsis_reg_TX_FIFO_11_INT_poll( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_11_INT_BIT_RP_BELOW_EXP_11_I_MSK,
                                             (value<<DSIS_REG_TX_FIFO_11_INT_BIT_RP_BELOW_EXP_11_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void sys_dsis_field_RP_IN_SYNC_11_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sys_dsis_field_RP_IN_SYNC_11_I_set_to_clear( sys_dsis_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sys_dsis_field_RP_IN_SYNC_11_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sys_dsis_field_RP_IN_SYNC_11_I_set_to_clear", value );

    /* (0x00001224 bits 0) field RP_IN_SYNC_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    sys_dsis_reg_TX_FIFO_11_INT_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           DSIS_REG_TX_FIFO_11_INT_BIT_RP_IN_SYNC_11_I_MSK,
                                                           DSIS_REG_TX_FIFO_11_INT_BIT_RP_IN_SYNC_11_I_OFF,
                                                           value);
}

static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_11_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sys_dsis_field_RP_IN_SYNC_11_I_get( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001224 bits 0) field RP_IN_SYNC_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    reg_value = sys_dsis_reg_TX_FIFO_11_INT_read(  b_ptr, h_ptr);
    value = (reg_value & DSIS_REG_TX_FIFO_11_INT_BIT_RP_IN_SYNC_11_I_MSK) >> DSIS_REG_TX_FIFO_11_INT_BIT_RP_IN_SYNC_11_I_OFF;
    IOLOG( "%s -> 0x%08x", "sys_dsis_field_RP_IN_SYNC_11_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_11_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_11_I_poll( sys_dsis_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sys_dsis_field_RP_IN_SYNC_11_I_poll", value );

    /* (0x00001224 bits 0) field RP_IN_SYNC_11_I of register PMC_DSIS_REG_TX_FIFO_11_INT */
    return sys_dsis_reg_TX_FIFO_11_INT_poll( b_ptr,
                                             h_ptr,
                                             DSIS_REG_TX_FIFO_11_INT_BIT_RP_IN_SYNC_11_I_MSK,
                                             (value<<DSIS_REG_TX_FIFO_11_INT_BIT_RP_IN_SYNC_11_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SYS_DSIS_IO_INLINE_H */
