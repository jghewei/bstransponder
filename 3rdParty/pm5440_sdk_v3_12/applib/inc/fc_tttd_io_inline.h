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
 *     and register accessor functions for the fc_tttd block
 *****************************************************************************/
#ifndef _FC_TTTD_IO_INLINE_H
#define _FC_TTTD_IO_INLINE_H

#include "cbrc_api.h"
#include "fc_tttd_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FC_TTTD_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for fc_tttd
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
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} fc_tttd_buffer_t;
static INLINE void fc_tttd_buffer_init( fc_tttd_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A ) ALWAYS_INLINE;
static INLINE void fc_tttd_buffer_init( fc_tttd_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x200) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "fc_tttd_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void fc_tttd_buffer_flush( fc_tttd_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void fc_tttd_buffer_flush( fc_tttd_buffer_t *b_ptr )
{
    IOLOG( "fc_tttd_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 fc_tttd_reg_read( fc_tttd_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_reg_read( fc_tttd_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x200),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg);
    return value;
}
static INLINE void fc_tttd_reg_write( fc_tttd_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_write( fc_tttd_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x200),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                         value);
}

static INLINE void fc_tttd_field_set( fc_tttd_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_set( fc_tttd_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x200),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void fc_tttd_action_on_write_field_set( fc_tttd_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_action_on_write_field_set( fc_tttd_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*0x200),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                         value<<ofs);
}

static INLINE void fc_tttd_burst_read( fc_tttd_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc_tttd_burst_read( fc_tttd_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x200),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                          len,
                          value);
}

static INLINE void fc_tttd_burst_write( fc_tttd_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc_tttd_burst_write( fc_tttd_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x200),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE fc_tttd_poll( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_poll( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x200),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for fc_tttd
 * ==================================================================================
 */

static INLINE void fc_tttd_reg_CFG_write( fc_tttd_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_CFG_write( fc_tttd_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttd_reg_CFG_write", value );
    fc_tttd_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTD_REG_CFG,
                       value);
}

static INLINE void fc_tttd_reg_CFG_field_set( fc_tttd_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_CFG_field_set( fc_tttd_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttd_reg_CFG_field_set", A, mask, ofs, value );
    fc_tttd_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTD_REG_CFG,
                       mask,
                       PMC_FC_TTTD_REG_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttd_reg_CFG_read( fc_tttd_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_reg_CFG_read( fc_tttd_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttd_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTD_REG_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttd_reg_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_write( fc_tttd_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_write( fc_tttd_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_write", value );
    fc_tttd_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB,
                       value);
}

static INLINE void fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set( fc_tttd_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set( fc_tttd_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set", A, mask, ofs, value );
    fc_tttd_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB,
                       mask,
                       PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_read( fc_tttd_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_read( fc_tttd_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttd_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_write( fc_tttd_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_write( fc_tttd_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_write", value );
    fc_tttd_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB,
                       value);
}

static INLINE void fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set( fc_tttd_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set( fc_tttd_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set", A, mask, ofs, value );
    fc_tttd_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB,
                       mask,
                       PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_read( fc_tttd_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_read( fc_tttd_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttd_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttd_reg_CNT_UPDATE_write( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_CNT_UPDATE_write( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttd_reg_CNT_UPDATE_write", value );
    fc_tttd_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_FC_TTTD_REG_CNT_UPDATE,
                       value);
}

static INLINE void fc_tttd_reg_CNT_UPDATE_field_set( fc_tttd_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_CNT_UPDATE_field_set( fc_tttd_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttd_reg_CNT_UPDATE_field_set", A, mask, ofs, value );
    fc_tttd_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_FC_TTTD_REG_CNT_UPDATE,
                       mask,
                       PMC_FC_TTTD_REG_CNT_UPDATE_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttd_reg_CNT_UPDATE_read( fc_tttd_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_reg_CNT_UPDATE_read( fc_tttd_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttd_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_FC_TTTD_REG_CNT_UPDATE);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttd_reg_CNT_UPDATE_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_reg_CNT_UPDATE_poll( fc_tttd_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_reg_CNT_UPDATE_poll( fc_tttd_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc_tttd_reg_CNT_UPDATE_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc_tttd_poll( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_FC_TTTD_REG_CNT_UPDATE,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void fc_tttd_reg_INT_EN_write( fc_tttd_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_INT_EN_write( fc_tttd_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttd_reg_INT_EN_write", value );
    fc_tttd_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_FC_TTTD_REG_INT_EN,
                       value);
}

static INLINE void fc_tttd_reg_INT_EN_field_set( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_INT_EN_field_set( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttd_reg_INT_EN_field_set", A, mask, ofs, value );
    fc_tttd_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_FC_TTTD_REG_INT_EN,
                       mask,
                       PMC_FC_TTTD_REG_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttd_reg_INT_EN_read( fc_tttd_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_reg_INT_EN_read( fc_tttd_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttd_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_FC_TTTD_REG_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttd_reg_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttd_reg_INT_write( fc_tttd_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_INT_write( fc_tttd_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttd_reg_INT_write", value );
    fc_tttd_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_FC_TTTD_REG_INT,
                       value);
}

static INLINE void fc_tttd_reg_INT_action_on_write_field_set( fc_tttd_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_reg_INT_action_on_write_field_set( fc_tttd_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttd_reg_INT_action_on_write_field_set", A, mask, ofs, value );
    fc_tttd_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_FC_TTTD_REG_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 fc_tttd_reg_INT_read( fc_tttd_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_reg_INT_read( fc_tttd_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttd_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_FC_TTTD_REG_INT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttd_reg_INT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_reg_INT_poll( fc_tttd_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 value,
                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                         UINT32 max_count,
                                                         UINT32 *num_failed_polls,
                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_reg_INT_poll( fc_tttd_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 value,
                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                         UINT32 max_count,
                                                         UINT32 *num_failed_polls,
                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc_tttd_reg_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc_tttd_poll( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_FC_TTTD_REG_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 fc_tttd_reg_CRC_24_ERR_CNT_read( fc_tttd_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_reg_CRC_24_ERR_CNT_read( fc_tttd_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttd_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_FC_TTTD_REG_CRC_24_ERR_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttd_reg_CRC_24_ERR_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc_tttd_reg_INVALID_BLOCK_HEADER_CNT_read( fc_tttd_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_reg_INVALID_BLOCK_HEADER_CNT_read( fc_tttd_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttd_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttd_reg_INVALID_BLOCK_HEADER_CNT_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void fc_tttd_field_FORCE_LF_set( fc_tttd_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_FORCE_LF_set( fc_tttd_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_FORCE_LF_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttd_field_FORCE_LF_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttd_field_FORCE_LF_set", A, value );

    /* (0x00050100 bits 25) field FORCE_LF of register PMC_FC_TTTD_REG_CFG */
    fc_tttd_reg_CFG_field_set( b_ptr,
                               h_ptr,
                               A,
                               FC_TTTD_REG_CFG_BIT_FORCE_LF_MSK,
                               FC_TTTD_REG_CFG_BIT_FORCE_LF_OFF,
                               value);
}

static INLINE UINT32 fc_tttd_field_FORCE_LF_get( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_FORCE_LF_get( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_FORCE_LF_get", A, 11);
    /* (0x00050100 bits 25) field FORCE_LF of register PMC_FC_TTTD_REG_CFG */
    reg_value = fc_tttd_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_CFG_BIT_FORCE_LF_MSK) >> FC_TTTD_REG_CFG_BIT_FORCE_LF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_FORCE_LF_get", A, value );

    return value;
}
static INLINE void fc_tttd_field_ZONE_THRES_set( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_ZONE_THRES_set( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_ZONE_THRES_set", A, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttd_field_ZONE_THRES_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttd_field_ZONE_THRES_set", A, value );

    /* (0x00050100 bits 9:8) bits 0:1 use field ZONE_THRES of register PMC_FC_TTTD_REG_CFG */
    fc_tttd_reg_CFG_field_set( b_ptr,
                               h_ptr,
                               A,
                               FC_TTTD_REG_CFG_BIT_ZONE_THRES_MSK,
                               FC_TTTD_REG_CFG_BIT_ZONE_THRES_OFF,
                               value);
}

static INLINE UINT32 fc_tttd_field_ZONE_THRES_get( fc_tttd_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_ZONE_THRES_get( fc_tttd_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_ZONE_THRES_get", A, 11);
    /* (0x00050100 bits 9:8) bits 0:1 use field ZONE_THRES of register PMC_FC_TTTD_REG_CFG */
    reg_value = fc_tttd_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_CFG_BIT_ZONE_THRES_MSK) >> FC_TTTD_REG_CFG_BIT_ZONE_THRES_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_ZONE_THRES_get", A, value );

    return value;
}
static INLINE void fc_tttd_field_range_ZONE_THRES_set( fc_tttd_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_range_ZONE_THRES_set( fc_tttd_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_range_ZONE_THRES_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttd_field_range_ZONE_THRES_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttd_field_range_ZONE_THRES_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttd_field_range_ZONE_THRES_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00050100 bits 9:8) bits 0:1 use field ZONE_THRES of register PMC_FC_TTTD_REG_CFG */
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
        /* (0x00050100 bits 9:8) bits 0:1 use field ZONE_THRES of register PMC_FC_TTTD_REG_CFG */
        fc_tttd_reg_CFG_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   subfield_mask << (FC_TTTD_REG_CFG_BIT_ZONE_THRES_OFF + subfield_offset),
                                   FC_TTTD_REG_CFG_BIT_ZONE_THRES_OFF + subfield_offset,
                                   value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttd_field_range_ZONE_THRES_get( fc_tttd_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_range_ZONE_THRES_get( fc_tttd_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_range_ZONE_THRES_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttd_field_range_ZONE_THRES_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttd_field_range_ZONE_THRES_get", stop_bit, 1 );
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
    /* (0x00050100 bits 9:8) bits 0:1 use field ZONE_THRES of register PMC_FC_TTTD_REG_CFG */
    reg_value = fc_tttd_reg_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_TTTD_REG_CFG_BIT_ZONE_THRES_MSK)
                  >> FC_TTTD_REG_CFG_BIT_ZONE_THRES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTD_REG_CFG_BIT_ZONE_THRES_MSK, FC_TTTD_REG_CFG_BIT_ZONE_THRES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttd_field_range_ZONE_THRES_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttd_lfield_LFOS_set( fc_tttd_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc_tttd_lfield_LFOS_set( fc_tttd_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value[2] )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_lfield_LFOS_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x 0x%08x", "fc_tttd_lfield_LFOS_set", A, value[1] , value[0] );

    /* (0x0005010c bits 31:0) bits 0:31 use field LFOS of register PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB */
    fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_MSK,
                                                       FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF,
                                                       value[0]);

    /* (0x00050110 bits 31:0) bits 32:63 use field LFOS of register PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB */
    fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_MSK,
                                                       FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF,
                                                       value[1]);
}

static INLINE void fc_tttd_lfield_LFOS_get( fc_tttd_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc_tttd_lfield_LFOS_get( fc_tttd_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_lfield_LFOS_get", A, 11);
    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0005010c bits 31:0) bits 0:31 use field LFOS of register PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB */
    reg_value = fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_MSK) >> FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF;
    value[0] |= field_value;

    /* (0x00050110 bits 31:0) bits 32:63 use field LFOS of register PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB */
    reg_value = fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_MSK) >> FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> A=%d 0x%08x 0x%08x", "fc_tttd_lfield_LFOS_get", A, value[1] , value[0] );

}
static INLINE void fc_tttd_lfield_range_LFOS_set( fc_tttd_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_lfield_range_LFOS_set( fc_tttd_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_lfield_range_LFOS_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttd_lfield_range_LFOS_set", stop_bit, start_bit );
    if (stop_bit > 63) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttd_lfield_range_LFOS_set", stop_bit, 63 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttd_lfield_range_LFOS_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0005010c bits 31:0) bits 0:31 use field LFOS of register PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB */
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
        /* (0x0005010c bits 31:0) bits 0:31 use field LFOS of register PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB */
        fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF + subfield_offset),
                                                           FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00050110 bits 31:0) bits 32:63 use field LFOS of register PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
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
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00050110 bits 31:0) bits 32:63 use field LFOS of register PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB */
        fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF + subfield_offset),
                                                           FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttd_lfield_range_LFOS_get( fc_tttd_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_lfield_range_LFOS_get( fc_tttd_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_lfield_range_LFOS_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttd_lfield_range_LFOS_get", stop_bit, start_bit );
    if (stop_bit > 63) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttd_lfield_range_LFOS_get", stop_bit, 63 );
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
    /* (0x0005010c bits 31:0) bits 0:31 use field LFOS of register PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB */
        reg_value = fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_LSB_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_MSK)
                      >> FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_MSK, FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 63) && (stop_bit >= 32)) {
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
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00050110 bits 31:0) bits 32:63 use field LFOS of register PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB */
        reg_value = fc_tttd_reg_LOCAL_FAULT_ORDERED_SET_MSB_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_MSK)
                      >> FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_MSK, FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttd_lfield_range_LFOS_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void fc_tttd_field_CNTR_UPDATE_set( fc_tttd_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_CNTR_UPDATE_set( fc_tttd_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_CNTR_UPDATE_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttd_field_CNTR_UPDATE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttd_field_CNTR_UPDATE_set", A, value );

    /* (0x00050120 bits 0) field CNTR_UPDATE of register PMC_FC_TTTD_REG_CNT_UPDATE */
    fc_tttd_reg_CNT_UPDATE_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      FC_TTTD_REG_CNT_UPDATE_BIT_CNTR_UPDATE_MSK,
                                      FC_TTTD_REG_CNT_UPDATE_BIT_CNTR_UPDATE_OFF,
                                      value);
}

static INLINE UINT32 fc_tttd_field_CNTR_UPDATE_get( fc_tttd_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_CNTR_UPDATE_get( fc_tttd_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_CNTR_UPDATE_get", A, 11);
    /* (0x00050120 bits 0) field CNTR_UPDATE of register PMC_FC_TTTD_REG_CNT_UPDATE */
    reg_value = fc_tttd_reg_CNT_UPDATE_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_CNT_UPDATE_BIT_CNTR_UPDATE_MSK) >> FC_TTTD_REG_CNT_UPDATE_BIT_CNTR_UPDATE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_CNTR_UPDATE_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void fc_tttd_field_INVALBLKHDR_E_set( fc_tttd_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_INVALBLKHDR_E_set( fc_tttd_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_INVALBLKHDR_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttd_field_INVALBLKHDR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttd_field_INVALBLKHDR_E_set", A, value );

    /* (0x00050108 bits 2) field INVALBLKHDR_E of register PMC_FC_TTTD_REG_INT_EN */
    fc_tttd_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  FC_TTTD_REG_INT_EN_BIT_INVALBLKHDR_E_MSK,
                                  FC_TTTD_REG_INT_EN_BIT_INVALBLKHDR_E_OFF,
                                  value);
}

static INLINE UINT32 fc_tttd_field_INVALBLKHDR_E_get( fc_tttd_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_INVALBLKHDR_E_get( fc_tttd_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_INVALBLKHDR_E_get", A, 11);
    /* (0x00050108 bits 2) field INVALBLKHDR_E of register PMC_FC_TTTD_REG_INT_EN */
    reg_value = fc_tttd_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_INT_EN_BIT_INVALBLKHDR_E_MSK) >> FC_TTTD_REG_INT_EN_BIT_INVALBLKHDR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_INVALBLKHDR_E_get", A, value );

    return value;
}
static INLINE void fc_tttd_field_UNDERRUN_E_set( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_UNDERRUN_E_set( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_UNDERRUN_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttd_field_UNDERRUN_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttd_field_UNDERRUN_E_set", A, value );

    /* (0x00050108 bits 1) field UNDERRUN_E of register PMC_FC_TTTD_REG_INT_EN */
    fc_tttd_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  FC_TTTD_REG_INT_EN_BIT_UNDERRUN_E_MSK,
                                  FC_TTTD_REG_INT_EN_BIT_UNDERRUN_E_OFF,
                                  value);
}

static INLINE UINT32 fc_tttd_field_UNDERRUN_E_get( fc_tttd_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_UNDERRUN_E_get( fc_tttd_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_UNDERRUN_E_get", A, 11);
    /* (0x00050108 bits 1) field UNDERRUN_E of register PMC_FC_TTTD_REG_INT_EN */
    reg_value = fc_tttd_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_INT_EN_BIT_UNDERRUN_E_MSK) >> FC_TTTD_REG_INT_EN_BIT_UNDERRUN_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_UNDERRUN_E_get", A, value );

    return value;
}
static INLINE void fc_tttd_field_CRC24ERR_E_set( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_CRC24ERR_E_set( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_CRC24ERR_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttd_field_CRC24ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttd_field_CRC24ERR_E_set", A, value );

    /* (0x00050108 bits 0) field CRC24ERR_E of register PMC_FC_TTTD_REG_INT_EN */
    fc_tttd_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  FC_TTTD_REG_INT_EN_BIT_CRC24ERR_E_MSK,
                                  FC_TTTD_REG_INT_EN_BIT_CRC24ERR_E_OFF,
                                  value);
}

static INLINE UINT32 fc_tttd_field_CRC24ERR_E_get( fc_tttd_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_CRC24ERR_E_get( fc_tttd_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_CRC24ERR_E_get", A, 11);
    /* (0x00050108 bits 0) field CRC24ERR_E of register PMC_FC_TTTD_REG_INT_EN */
    reg_value = fc_tttd_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_INT_EN_BIT_CRC24ERR_E_MSK) >> FC_TTTD_REG_INT_EN_BIT_CRC24ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_CRC24ERR_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void fc_tttd_field_INVALBLKHDR_I_set_to_clear( fc_tttd_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_INVALBLKHDR_I_set_to_clear( fc_tttd_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_INVALBLKHDR_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttd_field_INVALBLKHDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttd_field_INVALBLKHDR_I_set_to_clear", A, value );

    /* (0x00050104 bits 2) field INVALBLKHDR_I of register PMC_FC_TTTD_REG_INT */
    fc_tttd_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               FC_TTTD_REG_INT_BIT_INVALBLKHDR_I_MSK,
                                               FC_TTTD_REG_INT_BIT_INVALBLKHDR_I_OFF,
                                               value);
}

static INLINE UINT32 fc_tttd_field_INVALBLKHDR_I_get( fc_tttd_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_INVALBLKHDR_I_get( fc_tttd_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_INVALBLKHDR_I_get", A, 11);
    /* (0x00050104 bits 2) field INVALBLKHDR_I of register PMC_FC_TTTD_REG_INT */
    reg_value = fc_tttd_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_INT_BIT_INVALBLKHDR_I_MSK) >> FC_TTTD_REG_INT_BIT_INVALBLKHDR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_INVALBLKHDR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_field_INVALBLKHDR_I_poll( fc_tttd_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_field_INVALBLKHDR_I_poll( fc_tttd_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttd_field_INVALBLKHDR_I_poll", A, value );

    /* (0x00050104 bits 2) field INVALBLKHDR_I of register PMC_FC_TTTD_REG_INT */
    return fc_tttd_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_TTTD_REG_INT_BIT_INVALBLKHDR_I_MSK,
                                 (value<<FC_TTTD_REG_INT_BIT_INVALBLKHDR_I_OFF),
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);
}

static INLINE void fc_tttd_field_UNDERRUN_I_set_to_clear( fc_tttd_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_UNDERRUN_I_set_to_clear( fc_tttd_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_UNDERRUN_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttd_field_UNDERRUN_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttd_field_UNDERRUN_I_set_to_clear", A, value );

    /* (0x00050104 bits 1) field UNDERRUN_I of register PMC_FC_TTTD_REG_INT */
    fc_tttd_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               FC_TTTD_REG_INT_BIT_UNDERRUN_I_MSK,
                                               FC_TTTD_REG_INT_BIT_UNDERRUN_I_OFF,
                                               value);
}

static INLINE UINT32 fc_tttd_field_UNDERRUN_I_get( fc_tttd_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_UNDERRUN_I_get( fc_tttd_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_UNDERRUN_I_get", A, 11);
    /* (0x00050104 bits 1) field UNDERRUN_I of register PMC_FC_TTTD_REG_INT */
    reg_value = fc_tttd_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_INT_BIT_UNDERRUN_I_MSK) >> FC_TTTD_REG_INT_BIT_UNDERRUN_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_UNDERRUN_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_field_UNDERRUN_I_poll( fc_tttd_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_field_UNDERRUN_I_poll( fc_tttd_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttd_field_UNDERRUN_I_poll", A, value );

    /* (0x00050104 bits 1) field UNDERRUN_I of register PMC_FC_TTTD_REG_INT */
    return fc_tttd_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_TTTD_REG_INT_BIT_UNDERRUN_I_MSK,
                                 (value<<FC_TTTD_REG_INT_BIT_UNDERRUN_I_OFF),
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);
}

static INLINE void fc_tttd_field_CRC24ERR_I_set_to_clear( fc_tttd_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_CRC24ERR_I_set_to_clear( fc_tttd_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_CRC24ERR_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttd_field_CRC24ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttd_field_CRC24ERR_I_set_to_clear", A, value );

    /* (0x00050104 bits 0) field CRC24ERR_I of register PMC_FC_TTTD_REG_INT */
    fc_tttd_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               FC_TTTD_REG_INT_BIT_CRC24ERR_I_MSK,
                                               FC_TTTD_REG_INT_BIT_CRC24ERR_I_OFF,
                                               value);
}

static INLINE UINT32 fc_tttd_field_CRC24ERR_I_get( fc_tttd_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_CRC24ERR_I_get( fc_tttd_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_CRC24ERR_I_get", A, 11);
    /* (0x00050104 bits 0) field CRC24ERR_I of register PMC_FC_TTTD_REG_INT */
    reg_value = fc_tttd_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_INT_BIT_CRC24ERR_I_MSK) >> FC_TTTD_REG_INT_BIT_CRC24ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_CRC24ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_field_CRC24ERR_I_poll( fc_tttd_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_field_CRC24ERR_I_poll( fc_tttd_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttd_field_CRC24ERR_I_poll", A, value );

    /* (0x00050104 bits 0) field CRC24ERR_I of register PMC_FC_TTTD_REG_INT */
    return fc_tttd_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_TTTD_REG_INT_BIT_CRC24ERR_I_MSK,
                                 (value<<FC_TTTD_REG_INT_BIT_CRC24ERR_I_OFF),
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
static INLINE UINT32 fc_tttd_field_CRCERRCNT_get( fc_tttd_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_CRCERRCNT_get( fc_tttd_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_CRCERRCNT_get", A, 11);
    /* (0x00050118 bits 15:0) bits 0:15 use field CRCERRCNT of register PMC_FC_TTTD_REG_CRC_24_ERR_CNT */
    reg_value = fc_tttd_reg_CRC_24_ERR_CNT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_CRC_24_ERR_CNT_BIT_CRCERRCNT_MSK) >> FC_TTTD_REG_CRC_24_ERR_CNT_BIT_CRCERRCNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_CRCERRCNT_get", A, value );

    return value;
}
static INLINE UINT32 fc_tttd_field_range_CRCERRCNT_get( fc_tttd_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_range_CRCERRCNT_get( fc_tttd_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_range_CRCERRCNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttd_field_range_CRCERRCNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttd_field_range_CRCERRCNT_get", stop_bit, 15 );
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
    /* (0x00050118 bits 15:0) bits 0:15 use field CRCERRCNT of register PMC_FC_TTTD_REG_CRC_24_ERR_CNT */
    reg_value = fc_tttd_reg_CRC_24_ERR_CNT_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_TTTD_REG_CRC_24_ERR_CNT_BIT_CRCERRCNT_MSK)
                  >> FC_TTTD_REG_CRC_24_ERR_CNT_BIT_CRCERRCNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTD_REG_CRC_24_ERR_CNT_BIT_CRCERRCNT_MSK, FC_TTTD_REG_CRC_24_ERR_CNT_BIT_CRCERRCNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttd_field_range_CRCERRCNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc_tttd_field_INVALBLKHDRCNT_get( fc_tttd_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_INVALBLKHDRCNT_get( fc_tttd_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_INVALBLKHDRCNT_get", A, 11);
    /* (0x0005011c bits 15:0) bits 0:15 use field INVALBLKHDRCNT of register PMC_FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT */
    reg_value = fc_tttd_reg_INVALID_BLOCK_HEADER_CNT_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT_BIT_INVALBLKHDRCNT_MSK) >> FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT_BIT_INVALBLKHDRCNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_INVALBLKHDRCNT_get", A, value );

    return value;
}
static INLINE UINT32 fc_tttd_field_range_INVALBLKHDRCNT_get( fc_tttd_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_range_INVALBLKHDRCNT_get( fc_tttd_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_range_INVALBLKHDRCNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttd_field_range_INVALBLKHDRCNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttd_field_range_INVALBLKHDRCNT_get", stop_bit, 15 );
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
    /* (0x0005011c bits 15:0) bits 0:15 use field INVALBLKHDRCNT of register PMC_FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT */
    reg_value = fc_tttd_reg_INVALID_BLOCK_HEADER_CNT_read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT_BIT_INVALBLKHDRCNT_MSK)
                  >> FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT_BIT_INVALBLKHDRCNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT_BIT_INVALBLKHDRCNT_MSK, FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT_BIT_INVALBLKHDRCNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttd_field_range_INVALBLKHDRCNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc_tttd_field_TIP_get( fc_tttd_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_TIP_get( fc_tttd_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_TIP_get", A, 11);
    /* (0x00050120 bits 31) field TIP of register PMC_FC_TTTD_REG_CNT_UPDATE */
    reg_value = fc_tttd_reg_CNT_UPDATE_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_CNT_UPDATE_BIT_TIP_MSK) >> FC_TTTD_REG_CNT_UPDATE_BIT_TIP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_TIP_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_field_TIP_poll( fc_tttd_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttd_field_TIP_poll( fc_tttd_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttd_field_TIP_poll", A, value );

    /* (0x00050120 bits 31) field TIP of register PMC_FC_TTTD_REG_CNT_UPDATE */
    return fc_tttd_reg_CNT_UPDATE_poll( b_ptr,
                                        h_ptr,
                                        A,
                                        FC_TTTD_REG_CNT_UPDATE_BIT_TIP_MSK,
                                        (value<<FC_TTTD_REG_CNT_UPDATE_BIT_TIP_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset sw_reset
 * ==================================================================================
 */
static INLINE void fc_tttd_field_SW_RESET_set( fc_tttd_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttd_field_SW_RESET_set( fc_tttd_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_SW_RESET_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttd_field_SW_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttd_field_SW_RESET_set", A, value );

    /* (0x00050100 bits 0) field SW_RESET of register PMC_FC_TTTD_REG_CFG */
    fc_tttd_reg_CFG_field_set( b_ptr,
                               h_ptr,
                               A,
                               FC_TTTD_REG_CFG_BIT_SW_RESET_MSK,
                               FC_TTTD_REG_CFG_BIT_SW_RESET_OFF,
                               value);
}

static INLINE UINT32 fc_tttd_field_SW_RESET_get( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttd_field_SW_RESET_get( fc_tttd_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttd_field_SW_RESET_get", A, 11);
    /* (0x00050100 bits 0) field SW_RESET of register PMC_FC_TTTD_REG_CFG */
    reg_value = fc_tttd_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTD_REG_CFG_BIT_SW_RESET_MSK) >> FC_TTTD_REG_CFG_BIT_SW_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttd_field_SW_RESET_get", A, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC_TTTD_IO_INLINE_H */
