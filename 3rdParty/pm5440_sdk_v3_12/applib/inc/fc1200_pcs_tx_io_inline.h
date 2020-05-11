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
 *     and register accessor functions for the fc1200_pcs_tx block
 *****************************************************************************/
#ifndef _FC1200_PCS_TX_IO_INLINE_H
#define _FC1200_PCS_TX_IO_INLINE_H

#include "cbrc_api.h"
#include "fc1200_pcs_tx_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FC1200_PCS_TX_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for fc1200_pcs_tx
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
} fc1200_pcs_tx_buffer_t;
static INLINE void fc1200_pcs_tx_buffer_init( fc1200_pcs_tx_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_buffer_init( fc1200_pcs_tx_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x200) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "fc1200_pcs_tx_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void fc1200_pcs_tx_buffer_flush( fc1200_pcs_tx_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_buffer_flush( fc1200_pcs_tx_buffer_t *b_ptr )
{
    IOLOG( "fc1200_pcs_tx_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 fc1200_pcs_tx_reg_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mem_type,
                                             UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_read( fc1200_pcs_tx_buffer_t *b_ptr,
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
static INLINE void fc1200_pcs_tx_reg_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_reg_write( fc1200_pcs_tx_buffer_t *b_ptr,
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

static INLINE void fc1200_pcs_tx_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 mask,
                                            UINT32 unused_mask,
                                            UINT32 ofs,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
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

static INLINE void fc1200_pcs_tx_action_on_write_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mem_type,
                                                            UINT32 reg,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_action_on_write_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
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

static INLINE void fc1200_pcs_tx_burst_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_burst_read( fc1200_pcs_tx_buffer_t *b_ptr,
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

static INLINE void fc1200_pcs_tx_burst_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 len,
                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_burst_write( fc1200_pcs_tx_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_poll( fc1200_pcs_tx_buffer_t *b_ptr,
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
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_poll( fc1200_pcs_tx_buffer_t *b_ptr,
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
 *  register access functions for fc1200_pcs_tx
 * ==================================================================================
 */

static INLINE void fc1200_pcs_tx_reg_CFG_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_reg_CFG_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc1200_pcs_tx_reg_CFG_write", value );
    fc1200_pcs_tx_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_FC1200_PCS_TX_REG_CFG,
                             value);
}

static INLINE void fc1200_pcs_tx_reg_CFG_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_reg_CFG_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc1200_pcs_tx_reg_CFG_field_set", A, mask, ofs, value );
    fc1200_pcs_tx_field_set( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_FC1200_PCS_TX_REG_CFG,
                             mask,
                             PMC_FC1200_PCS_TX_REG_CFG_UNUSED_MASK,
                             ofs,
                             value);

}

static INLINE UINT32 fc1200_pcs_tx_reg_CFG_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_CFG_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_tx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_CONFIG,
                                        PMC_FC1200_PCS_TX_REG_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_tx_reg_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_write", value );
    fc1200_pcs_tx_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG,
                             value);
}

static INLINE void fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_field_set", A, mask, ofs, value );
    fc1200_pcs_tx_field_set( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG,
                             mask,
                             PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_UNUSED_MASK,
                             ofs,
                             value);

}

static INLINE UINT32 fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_tx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_CONFIG,
                                        PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc1200_pcs_tx_reg_PMON_CNT_STAT_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_reg_PMON_CNT_STAT_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc1200_pcs_tx_reg_PMON_CNT_STAT_write", value );
    fc1200_pcs_tx_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_FC1200_PCS_TX_REG_PMON_CNT_STAT,
                             value);
}

static INLINE void fc1200_pcs_tx_reg_PMON_CNT_STAT_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_reg_PMON_CNT_STAT_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc1200_pcs_tx_reg_PMON_CNT_STAT_field_set", A, mask, ofs, value );
    fc1200_pcs_tx_field_set( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_FC1200_PCS_TX_REG_PMON_CNT_STAT,
                             mask,
                             PMC_FC1200_PCS_TX_REG_PMON_CNT_STAT_UNUSED_MASK,
                             ofs,
                             value);

}

static INLINE UINT32 fc1200_pcs_tx_reg_PMON_CNT_STAT_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_PMON_CNT_STAT_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_tx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_TX_REG_PMON_CNT_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_tx_reg_PMON_CNT_STAT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_reg_PMON_CNT_STAT_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_reg_PMON_CNT_STAT_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc1200_pcs_tx_reg_PMON_CNT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc1200_pcs_tx_poll( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_FC1200_PCS_TX_REG_PMON_CNT_STAT,
                               mask,
                               value,
                               cmp,
                               max_count,
                               num_failed_polls,
                               delay_between_polls_in_microseconds);

}


static INLINE void fc1200_pcs_tx_reg_INT_EN_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_reg_INT_EN_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc1200_pcs_tx_reg_INT_EN_write", value );
    fc1200_pcs_tx_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_FC1200_PCS_TX_REG_INT_EN,
                             value);
}

static INLINE void fc1200_pcs_tx_reg_INT_EN_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_reg_INT_EN_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc1200_pcs_tx_reg_INT_EN_field_set", A, mask, ofs, value );
    fc1200_pcs_tx_field_set( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_FC1200_PCS_TX_REG_INT_EN,
                             mask,
                             PMC_FC1200_PCS_TX_REG_INT_EN_UNUSED_MASK,
                             ofs,
                             value);

}

static INLINE UINT32 fc1200_pcs_tx_reg_INT_EN_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_INT_EN_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_tx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_TX_REG_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_tx_reg_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE void fc1200_pcs_tx_reg_INT_INDICATION_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_reg_INT_INDICATION_write( fc1200_pcs_tx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc1200_pcs_tx_reg_INT_INDICATION_write", value );
    fc1200_pcs_tx_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_FC1200_PCS_TX_REG_INT_INDICATION,
                             value);
}

static INLINE void fc1200_pcs_tx_reg_INT_INDICATION_action_on_write_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_reg_INT_INDICATION_action_on_write_field_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc1200_pcs_tx_reg_INT_INDICATION_action_on_write_field_set", A, mask, ofs, value );
    fc1200_pcs_tx_action_on_write_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_STATUS,
                                             PMC_FC1200_PCS_TX_REG_INT_INDICATION,
                                             mask,
                                             ofs,
                                             value);

}

static INLINE UINT32 fc1200_pcs_tx_reg_INT_INDICATION_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_INT_INDICATION_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_tx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_TX_REG_INT_INDICATION);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_tx_reg_INT_INDICATION_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_reg_INT_INDICATION_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                          cbrc_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_reg_INT_INDICATION_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                          cbrc_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc1200_pcs_tx_reg_INT_INDICATION_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc1200_pcs_tx_poll( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_FC1200_PCS_TX_REG_INT_INDICATION,
                               mask,
                               value,
                               cmp,
                               max_count,
                               num_failed_polls,
                               delay_between_polls_in_microseconds);

}


static INLINE UINT32 fc1200_pcs_tx_reg_INT_STAT_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_INT_STAT_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_tx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_TX_REG_INT_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_tx_reg_INT_STAT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_reg_INT_STAT_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_reg_INT_STAT_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc1200_pcs_tx_reg_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc1200_pcs_tx_poll( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_FC1200_PCS_TX_REG_INT_STAT,
                               mask,
                               value,
                               cmp,
                               max_count,
                               num_failed_polls,
                               delay_between_polls_in_microseconds);

}


static INLINE UINT32 fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_LSB_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_LSB_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_tx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_LSB_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_MSB_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_MSB_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_tx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_MSB_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc1200_pcs_tx_reg_TX_ORDERED_SET_CNT_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_TX_ORDERED_SET_CNT_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_tx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_tx_reg_TX_ORDERED_SET_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc1200_pcs_tx_reg_ERR_CTRL_BLOCK_CNT_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_ERR_CTRL_BLOCK_CNT_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_tx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_tx_reg_ERR_CTRL_BLOCK_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CNT_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CNT_read( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_tx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CNT_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void fc1200_pcs_tx_field_CORRUPT_BLK_MODE_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_CORRUPT_BLK_MODE_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_CORRUPT_BLK_MODE_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_CORRUPT_BLK_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_CORRUPT_BLK_MODE_set", A, value );

    /* (0x00050000 bits 8) field CORRUPT_BLK_MODE of register PMC_FC1200_PCS_TX_REG_CFG */
    fc1200_pcs_tx_reg_CFG_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     FC1200_PCS_TX_REG_CFG_BIT_CORRUPT_BLK_MODE_MSK,
                                     FC1200_PCS_TX_REG_CFG_BIT_CORRUPT_BLK_MODE_OFF,
                                     value);
}

static INLINE UINT32 fc1200_pcs_tx_field_CORRUPT_BLK_MODE_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_CORRUPT_BLK_MODE_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_CORRUPT_BLK_MODE_get", A, 11);
    /* (0x00050000 bits 8) field CORRUPT_BLK_MODE of register PMC_FC1200_PCS_TX_REG_CFG */
    reg_value = fc1200_pcs_tx_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_TX_REG_CFG_BIT_CORRUPT_BLK_MODE_MSK) >> FC1200_PCS_TX_REG_CFG_BIT_CORRUPT_BLK_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_CORRUPT_BLK_MODE_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_tx_field_LOS_LF_ENA_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_LOS_LF_ENA_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LOS_LF_ENA_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_LOS_LF_ENA_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_LOS_LF_ENA_set", A, value );

    /* (0x00050000 bits 5) field LOS_LF_ENA of register PMC_FC1200_PCS_TX_REG_CFG */
    fc1200_pcs_tx_reg_CFG_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     FC1200_PCS_TX_REG_CFG_BIT_LOS_LF_ENA_MSK,
                                     FC1200_PCS_TX_REG_CFG_BIT_LOS_LF_ENA_OFF,
                                     value);
}

static INLINE UINT32 fc1200_pcs_tx_field_LOS_LF_ENA_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_LOS_LF_ENA_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LOS_LF_ENA_get", A, 11);
    /* (0x00050000 bits 5) field LOS_LF_ENA of register PMC_FC1200_PCS_TX_REG_CFG */
    reg_value = fc1200_pcs_tx_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_TX_REG_CFG_BIT_LOS_LF_ENA_MSK) >> FC1200_PCS_TX_REG_CFG_BIT_LOS_LF_ENA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_LOS_LF_ENA_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_tx_field_EXT_LF_ENA_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_EXT_LF_ENA_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_EXT_LF_ENA_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_EXT_LF_ENA_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_EXT_LF_ENA_set", A, value );

    /* (0x00050000 bits 4) field EXT_LF_ENA of register PMC_FC1200_PCS_TX_REG_CFG */
    fc1200_pcs_tx_reg_CFG_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     FC1200_PCS_TX_REG_CFG_BIT_EXT_LF_ENA_MSK,
                                     FC1200_PCS_TX_REG_CFG_BIT_EXT_LF_ENA_OFF,
                                     value);
}

static INLINE UINT32 fc1200_pcs_tx_field_EXT_LF_ENA_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_EXT_LF_ENA_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_EXT_LF_ENA_get", A, 11);
    /* (0x00050000 bits 4) field EXT_LF_ENA of register PMC_FC1200_PCS_TX_REG_CFG */
    reg_value = fc1200_pcs_tx_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_TX_REG_CFG_BIT_EXT_LF_ENA_MSK) >> FC1200_PCS_TX_REG_CFG_BIT_EXT_LF_ENA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_EXT_LF_ENA_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_tx_field_FORCE_LF_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_FORCE_LF_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_FORCE_LF_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_FORCE_LF_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_FORCE_LF_set", A, value );

    /* (0x00050000 bits 3) field FORCE_LF of register PMC_FC1200_PCS_TX_REG_CFG */
    fc1200_pcs_tx_reg_CFG_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     FC1200_PCS_TX_REG_CFG_BIT_FORCE_LF_MSK,
                                     FC1200_PCS_TX_REG_CFG_BIT_FORCE_LF_OFF,
                                     value);
}

static INLINE UINT32 fc1200_pcs_tx_field_FORCE_LF_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_FORCE_LF_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_FORCE_LF_get", A, 11);
    /* (0x00050000 bits 3) field FORCE_LF of register PMC_FC1200_PCS_TX_REG_CFG */
    reg_value = fc1200_pcs_tx_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_TX_REG_CFG_BIT_FORCE_LF_MSK) >> FC1200_PCS_TX_REG_CFG_BIT_FORCE_LF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_FORCE_LF_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_tx_field_PRBS31_ENA_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_PRBS31_ENA_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_PRBS31_ENA_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_PRBS31_ENA_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_PRBS31_ENA_set", A, value );

    /* (0x00050000 bits 1) field PRBS31_ENA of register PMC_FC1200_PCS_TX_REG_CFG */
    fc1200_pcs_tx_reg_CFG_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     FC1200_PCS_TX_REG_CFG_BIT_PRBS31_ENA_MSK,
                                     FC1200_PCS_TX_REG_CFG_BIT_PRBS31_ENA_OFF,
                                     value);
}

static INLINE UINT32 fc1200_pcs_tx_field_PRBS31_ENA_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_PRBS31_ENA_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_PRBS31_ENA_get", A, 11);
    /* (0x00050000 bits 1) field PRBS31_ENA of register PMC_FC1200_PCS_TX_REG_CFG */
    reg_value = fc1200_pcs_tx_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_TX_REG_CFG_BIT_PRBS31_ENA_MSK) >> FC1200_PCS_TX_REG_CFG_BIT_PRBS31_ENA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_PRBS31_ENA_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_tx_field_LF_O_CODE_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_LF_O_CODE_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LF_O_CODE_set", A, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_LF_O_CODE_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_LF_O_CODE_set", A, value );

    /* (0x00050008 bits 27:24) field LF_O_CODE of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG */
    fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_MSK,
                                                             FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_OFF,
                                                             value);
}

static INLINE UINT32 fc1200_pcs_tx_field_LF_O_CODE_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_LF_O_CODE_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LF_O_CODE_get", A, 11);
    /* (0x00050008 bits 27:24) field LF_O_CODE of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG */
    reg_value = fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_MSK) >> FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_LF_O_CODE_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_tx_field_range_LF_O_CODE_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_range_LF_O_CODE_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_range_LF_O_CODE_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_tx_field_range_LF_O_CODE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_tx_field_range_LF_O_CODE_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_tx_field_range_LF_O_CODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00050008 bits 27:24) field LF_O_CODE of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG */
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
        /* (0x00050008 bits 27:24) field LF_O_CODE of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG */
        fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_OFF + subfield_offset),
                                                                 FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 fc1200_pcs_tx_field_range_LF_O_CODE_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_range_LF_O_CODE_get( fc1200_pcs_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_range_LF_O_CODE_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_tx_field_range_LF_O_CODE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_tx_field_range_LF_O_CODE_get", stop_bit, 3 );
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
    /* (0x00050008 bits 27:24) field LF_O_CODE of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG */
    reg_value = fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_MSK)
                  >> FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_MSK, FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_tx_field_range_LF_O_CODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc1200_pcs_tx_field_LF_ORDERED_SET_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_LF_ORDERED_SET_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LF_ORDERED_SET_set", A, 11);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_LF_ORDERED_SET_set", value, 16777215);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_LF_ORDERED_SET_set", A, value );

    /* (0x00050008 bits 23:0) field LF_ORDERED_SET of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG */
    fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_MSK,
                                                             FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_OFF,
                                                             value);
}

static INLINE UINT32 fc1200_pcs_tx_field_LF_ORDERED_SET_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_LF_ORDERED_SET_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LF_ORDERED_SET_get", A, 11);
    /* (0x00050008 bits 23:0) field LF_ORDERED_SET of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG */
    reg_value = fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_MSK) >> FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_LF_ORDERED_SET_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_tx_field_range_LF_ORDERED_SET_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_range_LF_ORDERED_SET_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_range_LF_ORDERED_SET_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_tx_field_range_LF_ORDERED_SET_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_tx_field_range_LF_ORDERED_SET_set", stop_bit, 23 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_tx_field_range_LF_ORDERED_SET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00050008 bits 23:0) field LF_ORDERED_SET of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG */
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
        if (stop_bit < 23) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 23;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00050008 bits 23:0) field LF_ORDERED_SET of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG */
        fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_OFF + subfield_offset),
                                                                 FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 fc1200_pcs_tx_field_range_LF_ORDERED_SET_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_range_LF_ORDERED_SET_get( fc1200_pcs_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_range_LF_ORDERED_SET_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_tx_field_range_LF_ORDERED_SET_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_tx_field_range_LF_ORDERED_SET_get", stop_bit, 23 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 23) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 23;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00050008 bits 23:0) field LF_ORDERED_SET of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG */
    reg_value = fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CFG_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_MSK)
                  >> FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_MSK, FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_tx_field_range_LF_ORDERED_SET_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void fc1200_pcs_tx_field_PMON_UPDATE_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_PMON_UPDATE_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_PMON_UPDATE_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_PMON_UPDATE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_PMON_UPDATE_set", A, value );

    /* (0x00050004 bits 0) field PMON_UPDATE of register PMC_FC1200_PCS_TX_REG_PMON_CNT_STAT */
    fc1200_pcs_tx_reg_PMON_CNT_STAT_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_MSK,
                                               FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_OFF,
                                               value);
}

static INLINE UINT32 fc1200_pcs_tx_field_PMON_UPDATE_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_PMON_UPDATE_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_PMON_UPDATE_get", A, 11);
    /* (0x00050004 bits 0) field PMON_UPDATE of register PMC_FC1200_PCS_TX_REG_PMON_CNT_STAT */
    reg_value = fc1200_pcs_tx_reg_PMON_CNT_STAT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_MSK) >> FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_PMON_UPDATE_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void fc1200_pcs_tx_field_BUF_UNDERRUN_E_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_BUF_UNDERRUN_E_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_BUF_UNDERRUN_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_BUF_UNDERRUN_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_BUF_UNDERRUN_E_set", A, value );

    /* (0x00050020 bits 1) field BUF_UNDERRUN_E of register PMC_FC1200_PCS_TX_REG_INT_EN */
    fc1200_pcs_tx_reg_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        FC1200_PCS_TX_REG_INT_EN_BIT_BUF_UNDERRUN_E_MSK,
                                        FC1200_PCS_TX_REG_INT_EN_BIT_BUF_UNDERRUN_E_OFF,
                                        value);
}

static INLINE UINT32 fc1200_pcs_tx_field_BUF_UNDERRUN_E_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_BUF_UNDERRUN_E_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_BUF_UNDERRUN_E_get", A, 11);
    /* (0x00050020 bits 1) field BUF_UNDERRUN_E of register PMC_FC1200_PCS_TX_REG_INT_EN */
    reg_value = fc1200_pcs_tx_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_TX_REG_INT_EN_BIT_BUF_UNDERRUN_E_MSK) >> FC1200_PCS_TX_REG_INT_EN_BIT_BUF_UNDERRUN_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_BUF_UNDERRUN_E_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_tx_field_LOCAL_FAULT_E_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_LOCAL_FAULT_E_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LOCAL_FAULT_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_LOCAL_FAULT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_LOCAL_FAULT_E_set", A, value );

    /* (0x00050020 bits 0) field LOCAL_FAULT_E of register PMC_FC1200_PCS_TX_REG_INT_EN */
    fc1200_pcs_tx_reg_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        FC1200_PCS_TX_REG_INT_EN_BIT_LOCAL_FAULT_E_MSK,
                                        FC1200_PCS_TX_REG_INT_EN_BIT_LOCAL_FAULT_E_OFF,
                                        value);
}

static INLINE UINT32 fc1200_pcs_tx_field_LOCAL_FAULT_E_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_LOCAL_FAULT_E_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LOCAL_FAULT_E_get", A, 11);
    /* (0x00050020 bits 0) field LOCAL_FAULT_E of register PMC_FC1200_PCS_TX_REG_INT_EN */
    reg_value = fc1200_pcs_tx_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_TX_REG_INT_EN_BIT_LOCAL_FAULT_E_MSK) >> FC1200_PCS_TX_REG_INT_EN_BIT_LOCAL_FAULT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_LOCAL_FAULT_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void fc1200_pcs_tx_field_BUF_UNDERRUN_I_set_to_clear( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_BUF_UNDERRUN_I_set_to_clear( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_BUF_UNDERRUN_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_BUF_UNDERRUN_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_BUF_UNDERRUN_I_set_to_clear", A, value );

    /* (0x00050024 bits 1) field BUF_UNDERRUN_I of register PMC_FC1200_PCS_TX_REG_INT_INDICATION */
    fc1200_pcs_tx_reg_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                FC1200_PCS_TX_REG_INT_INDICATION_BIT_BUF_UNDERRUN_I_MSK,
                                                                FC1200_PCS_TX_REG_INT_INDICATION_BIT_BUF_UNDERRUN_I_OFF,
                                                                value);
}

static INLINE UINT32 fc1200_pcs_tx_field_BUF_UNDERRUN_I_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_BUF_UNDERRUN_I_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_BUF_UNDERRUN_I_get", A, 11);
    /* (0x00050024 bits 1) field BUF_UNDERRUN_I of register PMC_FC1200_PCS_TX_REG_INT_INDICATION */
    reg_value = fc1200_pcs_tx_reg_INT_INDICATION_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & FC1200_PCS_TX_REG_INT_INDICATION_BIT_BUF_UNDERRUN_I_MSK) >> FC1200_PCS_TX_REG_INT_INDICATION_BIT_BUF_UNDERRUN_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_BUF_UNDERRUN_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_BUF_UNDERRUN_I_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_BUF_UNDERRUN_I_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_tx_field_BUF_UNDERRUN_I_poll", A, value );

    /* (0x00050024 bits 1) field BUF_UNDERRUN_I of register PMC_FC1200_PCS_TX_REG_INT_INDICATION */
    return fc1200_pcs_tx_reg_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  FC1200_PCS_TX_REG_INT_INDICATION_BIT_BUF_UNDERRUN_I_MSK,
                                                  (value<<FC1200_PCS_TX_REG_INT_INDICATION_BIT_BUF_UNDERRUN_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void fc1200_pcs_tx_field_LOCAL_FAULT_I_set_to_clear( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_LOCAL_FAULT_I_set_to_clear( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LOCAL_FAULT_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_LOCAL_FAULT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_LOCAL_FAULT_I_set_to_clear", A, value );

    /* (0x00050024 bits 0) field LOCAL_FAULT_I of register PMC_FC1200_PCS_TX_REG_INT_INDICATION */
    fc1200_pcs_tx_reg_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                FC1200_PCS_TX_REG_INT_INDICATION_BIT_LOCAL_FAULT_I_MSK,
                                                                FC1200_PCS_TX_REG_INT_INDICATION_BIT_LOCAL_FAULT_I_OFF,
                                                                value);
}

static INLINE UINT32 fc1200_pcs_tx_field_LOCAL_FAULT_I_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_LOCAL_FAULT_I_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LOCAL_FAULT_I_get", A, 11);
    /* (0x00050024 bits 0) field LOCAL_FAULT_I of register PMC_FC1200_PCS_TX_REG_INT_INDICATION */
    reg_value = fc1200_pcs_tx_reg_INT_INDICATION_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & FC1200_PCS_TX_REG_INT_INDICATION_BIT_LOCAL_FAULT_I_MSK) >> FC1200_PCS_TX_REG_INT_INDICATION_BIT_LOCAL_FAULT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_LOCAL_FAULT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_LOCAL_FAULT_I_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_LOCAL_FAULT_I_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_tx_field_LOCAL_FAULT_I_poll", A, value );

    /* (0x00050024 bits 0) field LOCAL_FAULT_I of register PMC_FC1200_PCS_TX_REG_INT_INDICATION */
    return fc1200_pcs_tx_reg_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  FC1200_PCS_TX_REG_INT_INDICATION_BIT_LOCAL_FAULT_I_MSK,
                                                  (value<<FC1200_PCS_TX_REG_INT_INDICATION_BIT_LOCAL_FAULT_I_OFF),
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
static INLINE UINT32 fc1200_pcs_tx_field_BUF_UNDERRUN_V_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_BUF_UNDERRUN_V_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_BUF_UNDERRUN_V_get", A, 11);
    /* (0x00050028 bits 1) field BUF_UNDERRUN_V of register PMC_FC1200_PCS_TX_REG_INT_STAT */
    reg_value = fc1200_pcs_tx_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_TX_REG_INT_STAT_BIT_BUF_UNDERRUN_V_MSK) >> FC1200_PCS_TX_REG_INT_STAT_BIT_BUF_UNDERRUN_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_BUF_UNDERRUN_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_BUF_UNDERRUN_V_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_BUF_UNDERRUN_V_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_tx_field_BUF_UNDERRUN_V_poll", A, value );

    /* (0x00050028 bits 1) field BUF_UNDERRUN_V of register PMC_FC1200_PCS_TX_REG_INT_STAT */
    return fc1200_pcs_tx_reg_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            FC1200_PCS_TX_REG_INT_STAT_BIT_BUF_UNDERRUN_V_MSK,
                                            (value<<FC1200_PCS_TX_REG_INT_STAT_BIT_BUF_UNDERRUN_V_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc1200_pcs_tx_field_LOCAL_FAULT_V_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_LOCAL_FAULT_V_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LOCAL_FAULT_V_get", A, 11);
    /* (0x00050028 bits 0) field LOCAL_FAULT_V of register PMC_FC1200_PCS_TX_REG_INT_STAT */
    reg_value = fc1200_pcs_tx_reg_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_TX_REG_INT_STAT_BIT_LOCAL_FAULT_V_MSK) >> FC1200_PCS_TX_REG_INT_STAT_BIT_LOCAL_FAULT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_LOCAL_FAULT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_LOCAL_FAULT_V_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_LOCAL_FAULT_V_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_tx_field_LOCAL_FAULT_V_poll", A, value );

    /* (0x00050028 bits 0) field LOCAL_FAULT_V of register PMC_FC1200_PCS_TX_REG_INT_STAT */
    return fc1200_pcs_tx_reg_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            FC1200_PCS_TX_REG_INT_STAT_BIT_LOCAL_FAULT_V_MSK,
                                            (value<<FC1200_PCS_TX_REG_INT_STAT_BIT_LOCAL_FAULT_V_OFF),
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
static INLINE UINT32 fc1200_pcs_tx_field_TIP_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_TIP_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_TIP_get", A, 11);
    /* (0x00050004 bits 8) field TIP of register PMC_FC1200_PCS_TX_REG_PMON_CNT_STAT */
    reg_value = fc1200_pcs_tx_reg_PMON_CNT_STAT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_TIP_MSK) >> FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_TIP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_TIP_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_TIP_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_tx_field_TIP_poll( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_tx_field_TIP_poll", A, value );

    /* (0x00050004 bits 8) field TIP of register PMC_FC1200_PCS_TX_REG_PMON_CNT_STAT */
    return fc1200_pcs_tx_reg_PMON_CNT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_TIP_MSK,
                                                 (value<<FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_TIP_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void fc1200_pcs_tx_lfield_DATA_BYTE_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_lfield_DATA_BYTE_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_lfield_DATA_BYTE_CNT_get", A, 11);
    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0005000c bits 31:0) bits 0:31 use field DATA_BYTE_CNT of register PMC_FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB */
    reg_value = fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_LSB_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_MSK) >> FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_OFF;
    value[0] |= field_value;

    /* (0x00050010 bits 7:0) bits 32:39 use field DATA_BYTE_CNT of register PMC_FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB */
    reg_value = fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_MSK) >> FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> A=%d 0x%08x 0x%08x", "fc1200_pcs_tx_lfield_DATA_BYTE_CNT_get", A, value[1] , value[0] );

}
static INLINE UINT32 fc1200_pcs_tx_lfield_range_DATA_BYTE_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_lfield_range_DATA_BYTE_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_lfield_range_DATA_BYTE_CNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_tx_lfield_range_DATA_BYTE_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_tx_lfield_range_DATA_BYTE_CNT_get", stop_bit, 39 );
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
    /* (0x0005000c bits 31:0) bits 0:31 use field DATA_BYTE_CNT of register PMC_FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB */
        reg_value = fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_LSB_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
        field_value = (reg_value & FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_MSK)
                      >> FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_MSK, FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 39) && (stop_bit >= 32)) {
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
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00050010 bits 7:0) bits 32:39 use field DATA_BYTE_CNT of register PMC_FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB */
        reg_value = fc1200_pcs_tx_reg_TX_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
        field_value = (reg_value & FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_MSK)
                      >> FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_MSK, FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_tx_lfield_range_DATA_BYTE_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_tx_field_ORDERED_SET_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_ORDERED_SET_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_ORDERED_SET_CNT_get", A, 11);
    /* (0x00050014 bits 31:0) bits 0:31 use field ORDERED_SET_CNT of register PMC_FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT */
    reg_value = fc1200_pcs_tx_reg_TX_ORDERED_SET_CNT_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_MSK) >> FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_ORDERED_SET_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_tx_field_range_ORDERED_SET_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_range_ORDERED_SET_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_range_ORDERED_SET_CNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_tx_field_range_ORDERED_SET_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_tx_field_range_ORDERED_SET_CNT_get", stop_bit, 31 );
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
    /* (0x00050014 bits 31:0) bits 0:31 use field ORDERED_SET_CNT of register PMC_FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT */
    reg_value = fc1200_pcs_tx_reg_TX_ORDERED_SET_CNT_read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_MSK)
                  >> FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_MSK, FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_tx_field_range_ORDERED_SET_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_tx_field_ERROR_BLK_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_ERROR_BLK_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_ERROR_BLK_CNT_get", A, 11);
    /* (0x00050018 bits 31:0) bits 0:31 use field ERROR_BLK_CNT of register PMC_FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT */
    reg_value = fc1200_pcs_tx_reg_ERR_CTRL_BLOCK_CNT_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT_BIT_ERROR_BLK_CNT_MSK) >> FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT_BIT_ERROR_BLK_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_ERROR_BLK_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_tx_field_range_ERROR_BLK_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_range_ERROR_BLK_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_range_ERROR_BLK_CNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_tx_field_range_ERROR_BLK_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_tx_field_range_ERROR_BLK_CNT_get", stop_bit, 31 );
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
    /* (0x00050018 bits 31:0) bits 0:31 use field ERROR_BLK_CNT of register PMC_FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT */
    reg_value = fc1200_pcs_tx_reg_ERR_CTRL_BLOCK_CNT_read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT_BIT_ERROR_BLK_CNT_MSK)
                  >> FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT_BIT_ERROR_BLK_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT_BIT_ERROR_BLK_CNT_MSK, FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT_BIT_ERROR_BLK_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_tx_field_range_ERROR_BLK_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_tx_field_LF_OS_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_LF_OS_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_LF_OS_CNT_get", A, 11);
    /* (0x0005001c bits 31:0) bits 0:31 use field LF_OS_CNT of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT */
    reg_value = fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CNT_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT_BIT_LF_OS_CNT_MSK) >> FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT_BIT_LF_OS_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_LF_OS_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_tx_field_range_LF_OS_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_range_LF_OS_CNT_get( fc1200_pcs_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_range_LF_OS_CNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_tx_field_range_LF_OS_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_tx_field_range_LF_OS_CNT_get", stop_bit, 31 );
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
    /* (0x0005001c bits 31:0) bits 0:31 use field LF_OS_CNT of register PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT */
    reg_value = fc1200_pcs_tx_reg_LOCAL_FAULT_ORDERED_SET_CNT_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT_BIT_LF_OS_CNT_MSK)
                  >> FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT_BIT_LF_OS_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT_BIT_LF_OS_CNT_MSK, FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT_BIT_LF_OS_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_tx_field_range_LF_OS_CNT_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset sw_reset
 * ==================================================================================
 */
static INLINE void fc1200_pcs_tx_field_SW_RESET_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_tx_field_SW_RESET_set( fc1200_pcs_tx_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_SW_RESET_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_tx_field_SW_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_tx_field_SW_RESET_set", A, value );

    /* (0x00050000 bits 0) field SW_RESET of register PMC_FC1200_PCS_TX_REG_CFG */
    fc1200_pcs_tx_reg_CFG_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     FC1200_PCS_TX_REG_CFG_BIT_SW_RESET_MSK,
                                     FC1200_PCS_TX_REG_CFG_BIT_SW_RESET_OFF,
                                     value);
}

static INLINE UINT32 fc1200_pcs_tx_field_SW_RESET_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_tx_field_SW_RESET_get( fc1200_pcs_tx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_tx_field_SW_RESET_get", A, 11);
    /* (0x00050000 bits 0) field SW_RESET of register PMC_FC1200_PCS_TX_REG_CFG */
    reg_value = fc1200_pcs_tx_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_TX_REG_CFG_BIT_SW_RESET_MSK) >> FC1200_PCS_TX_REG_CFG_BIT_SW_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_tx_field_SW_RESET_get", A, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC1200_PCS_TX_IO_INLINE_H */
