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
 *     and register accessor functions for the fc1200_pcs_rx block
 *****************************************************************************/
#ifndef _FC1200_PCS_RX_IO_INLINE_H
#define _FC1200_PCS_RX_IO_INLINE_H

#include "cbrc_api.h"
#include "fc1200_pcs_rx_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FC1200_PCS_RX_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for fc1200_pcs_rx
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
} fc1200_pcs_rx_buffer_t;
static INLINE void fc1200_pcs_rx_buffer_init( fc1200_pcs_rx_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_buffer_init( fc1200_pcs_rx_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x200) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "fc1200_pcs_rx_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void fc1200_pcs_rx_buffer_flush( fc1200_pcs_rx_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_buffer_flush( fc1200_pcs_rx_buffer_t *b_ptr )
{
    IOLOG( "fc1200_pcs_rx_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 fc1200_pcs_rx_reg_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mem_type,
                                             UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_read( fc1200_pcs_rx_buffer_t *b_ptr,
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
static INLINE void fc1200_pcs_rx_reg_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_reg_write( fc1200_pcs_rx_buffer_t *b_ptr,
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

static INLINE void fc1200_pcs_rx_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 mask,
                                            UINT32 unused_mask,
                                            UINT32 ofs,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
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

static INLINE void fc1200_pcs_rx_action_on_write_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mem_type,
                                                            UINT32 reg,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_action_on_write_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
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

static INLINE void fc1200_pcs_rx_burst_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_burst_read( fc1200_pcs_rx_buffer_t *b_ptr,
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

static INLINE void fc1200_pcs_rx_burst_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 len,
                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_burst_write( fc1200_pcs_rx_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_poll( fc1200_pcs_rx_buffer_t *b_ptr,
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
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_poll( fc1200_pcs_rx_buffer_t *b_ptr,
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
 *  register access functions for fc1200_pcs_rx
 * ==================================================================================
 */

static INLINE void fc1200_pcs_rx_reg_CFG_REG_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_reg_CFG_REG_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc1200_pcs_rx_reg_CFG_REG_write", value );
    fc1200_pcs_rx_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_FC1200_PCS_RX_REG_CFG_REG,
                             value);
}

static INLINE void fc1200_pcs_rx_reg_CFG_REG_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_reg_CFG_REG_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc1200_pcs_rx_reg_CFG_REG_field_set", A, mask, ofs, value );
    fc1200_pcs_rx_field_set( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_FC1200_PCS_RX_REG_CFG_REG,
                             mask,
                             PMC_FC1200_PCS_RX_REG_CFG_REG_UNUSED_MASK,
                             ofs,
                             value);

}

static INLINE UINT32 fc1200_pcs_rx_reg_CFG_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_CFG_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_CONFIG,
                                        PMC_FC1200_PCS_RX_REG_CFG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_CFG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_write", value );
    fc1200_pcs_rx_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_FC1200_PCS_RX_REG_HIGH_BER_CFG_REG,
                             value);
}

static INLINE void fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_field_set", A, mask, ofs, value );
    fc1200_pcs_rx_field_set( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_CONFIG,
                             PMC_FC1200_PCS_RX_REG_HIGH_BER_CFG_REG,
                             mask,
                             PMC_FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_UNUSED_MASK,
                             ofs,
                             value);

}

static INLINE UINT32 fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_CONFIG,
                                        PMC_FC1200_PCS_RX_REG_HIGH_BER_CFG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc1200_pcs_rx_reg_PMON_CNT_STAT_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_reg_PMON_CNT_STAT_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc1200_pcs_rx_reg_PMON_CNT_STAT_write", value );
    fc1200_pcs_rx_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_FC1200_PCS_RX_REG_PMON_CNT_STAT,
                             value);
}

static INLINE void fc1200_pcs_rx_reg_PMON_CNT_STAT_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_reg_PMON_CNT_STAT_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc1200_pcs_rx_reg_PMON_CNT_STAT_field_set", A, mask, ofs, value );
    fc1200_pcs_rx_field_set( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_FC1200_PCS_RX_REG_PMON_CNT_STAT,
                             mask,
                             PMC_FC1200_PCS_RX_REG_PMON_CNT_STAT_UNUSED_MASK,
                             ofs,
                             value);

}

static INLINE UINT32 fc1200_pcs_rx_reg_PMON_CNT_STAT_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_PMON_CNT_STAT_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_RX_REG_PMON_CNT_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_PMON_CNT_STAT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_reg_PMON_CNT_STAT_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_reg_PMON_CNT_STAT_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc1200_pcs_rx_reg_PMON_CNT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc1200_pcs_rx_poll( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_FC1200_PCS_RX_REG_PMON_CNT_STAT,
                               mask,
                               value,
                               cmp,
                               max_count,
                               num_failed_polls,
                               delay_between_polls_in_microseconds);

}


static INLINE void fc1200_pcs_rx_reg_INT_EN_REG_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_reg_INT_EN_REG_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc1200_pcs_rx_reg_INT_EN_REG_write", value );
    fc1200_pcs_rx_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_FC1200_PCS_RX_REG_INT_EN_REG,
                             value);
}

static INLINE void fc1200_pcs_rx_reg_INT_EN_REG_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_reg_INT_EN_REG_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc1200_pcs_rx_reg_INT_EN_REG_field_set", A, mask, ofs, value );
    fc1200_pcs_rx_field_set( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_FC1200_PCS_RX_REG_INT_EN_REG,
                             mask,
                             PMC_FC1200_PCS_RX_REG_INT_EN_REG_UNUSED_MASK,
                             ofs,
                             value);

}

static INLINE UINT32 fc1200_pcs_rx_reg_INT_EN_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_INT_EN_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_RX_REG_INT_EN_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_INT_EN_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc1200_pcs_rx_reg_INT_INDICATION_REG_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_reg_INT_INDICATION_REG_write( fc1200_pcs_rx_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc1200_pcs_rx_reg_INT_INDICATION_REG_write", value );
    fc1200_pcs_rx_reg_write( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG,
                             value);
}

static INLINE void fc1200_pcs_rx_reg_INT_INDICATION_REG_action_on_write_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                                   cbrc_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_reg_INT_INDICATION_REG_action_on_write_field_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                                   cbrc_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc1200_pcs_rx_reg_INT_INDICATION_REG_action_on_write_field_set", A, mask, ofs, value );
    fc1200_pcs_rx_action_on_write_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_STATUS,
                                             PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG,
                                             mask,
                                             ofs,
                                             value);

}

static INLINE UINT32 fc1200_pcs_rx_reg_INT_INDICATION_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_INT_INDICATION_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_INT_INDICATION_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_reg_INT_INDICATION_REG_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_reg_INT_INDICATION_REG_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc1200_pcs_rx_reg_INT_INDICATION_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc1200_pcs_rx_poll( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG,
                               mask,
                               value,
                               cmp,
                               max_count,
                               num_failed_polls,
                               delay_between_polls_in_microseconds);

}


static INLINE UINT32 fc1200_pcs_rx_reg_INT_STAT_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_INT_STAT_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_RX_REG_INT_STAT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_INT_STAT_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_reg_INT_STAT_REG_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_reg_INT_STAT_REG_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc1200_pcs_rx_reg_INT_STAT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc1200_pcs_rx_poll( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_FC1200_PCS_RX_REG_INT_STAT_REG,
                               mask,
                               value,
                               cmp,
                               max_count,
                               num_failed_polls,
                               delay_between_polls_in_microseconds);

}


static INLINE UINT32 fc1200_pcs_rx_reg_STAT_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_STAT_REG_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_RX_REG_STAT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_STAT_REG_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc1200_pcs_rx_reg_ERR_CNTS_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_ERR_CNTS_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_RX_REG_ERR_CNTS);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_ERR_CNTS_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc1200_pcs_rx_reg_BLOCK_ERR_CNTS_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_BLOCK_ERR_CNTS_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_RX_REG_BLOCK_ERR_CNTS);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_BLOCK_ERR_CNTS_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_LSB_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_LSB_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_LSB_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_MSB_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_MSB_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_MSB_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc1200_pcs_rx_reg_RXD_ORDERED_SET_CNT_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_reg_RXD_ORDERED_SET_CNT_read( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc1200_pcs_rx_reg_read( b_ptr,
                                        h_ptr,
                                        A,
                                        MEM_TYPE_STATUS,
                                        PMC_FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc1200_pcs_rx_reg_RXD_ORDERED_SET_CNT_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_set", A, value );

    /* (0x0002f000 bits 4) field RESTORE_INVALID_IDLE of register PMC_FC1200_PCS_RX_REG_CFG_REG */
    fc1200_pcs_rx_reg_CFG_REG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         FC1200_PCS_RX_REG_CFG_REG_BIT_RESTORE_INVALID_IDLE_MSK,
                                         FC1200_PCS_RX_REG_CFG_REG_BIT_RESTORE_INVALID_IDLE_OFF,
                                         value);
}

static INLINE UINT32 fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_get", A, 11);
    /* (0x0002f000 bits 4) field RESTORE_INVALID_IDLE of register PMC_FC1200_PCS_RX_REG_CFG_REG */
    reg_value = fc1200_pcs_rx_reg_CFG_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_RX_REG_CFG_REG_BIT_RESTORE_INVALID_IDLE_MSK) >> FC1200_PCS_RX_REG_CFG_REG_BIT_RESTORE_INVALID_IDLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_RESTORE_INVALID_IDLE_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_rx_field_PRBS31_ENA_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_PRBS31_ENA_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_PRBS31_ENA_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_PRBS31_ENA_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_PRBS31_ENA_set", A, value );

    /* (0x0002f000 bits 2) field PRBS31_ENA of register PMC_FC1200_PCS_RX_REG_CFG_REG */
    fc1200_pcs_rx_reg_CFG_REG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         FC1200_PCS_RX_REG_CFG_REG_BIT_PRBS31_ENA_MSK,
                                         FC1200_PCS_RX_REG_CFG_REG_BIT_PRBS31_ENA_OFF,
                                         value);
}

static INLINE UINT32 fc1200_pcs_rx_field_PRBS31_ENA_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_PRBS31_ENA_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_PRBS31_ENA_get", A, 11);
    /* (0x0002f000 bits 2) field PRBS31_ENA of register PMC_FC1200_PCS_RX_REG_CFG_REG */
    reg_value = fc1200_pcs_rx_reg_CFG_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_RX_REG_CFG_REG_BIT_PRBS31_ENA_MSK) >> FC1200_PCS_RX_REG_CFG_REG_BIT_PRBS31_ENA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_PRBS31_ENA_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_rx_field_DESCRAMBLER_ENA_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_DESCRAMBLER_ENA_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_DESCRAMBLER_ENA_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_DESCRAMBLER_ENA_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_DESCRAMBLER_ENA_set", A, value );

    /* (0x0002f000 bits 1) field DESCRAMBLER_ENA of register PMC_FC1200_PCS_RX_REG_CFG_REG */
    fc1200_pcs_rx_reg_CFG_REG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         FC1200_PCS_RX_REG_CFG_REG_BIT_DESCRAMBLER_ENA_MSK,
                                         FC1200_PCS_RX_REG_CFG_REG_BIT_DESCRAMBLER_ENA_OFF,
                                         value);
}

static INLINE UINT32 fc1200_pcs_rx_field_DESCRAMBLER_ENA_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_DESCRAMBLER_ENA_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_DESCRAMBLER_ENA_get", A, 11);
    /* (0x0002f000 bits 1) field DESCRAMBLER_ENA of register PMC_FC1200_PCS_RX_REG_CFG_REG */
    reg_value = fc1200_pcs_rx_reg_CFG_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_RX_REG_CFG_REG_BIT_DESCRAMBLER_ENA_MSK) >> FC1200_PCS_RX_REG_CFG_REG_BIT_DESCRAMBLER_ENA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_DESCRAMBLER_ENA_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_rx_field_COUNT_125US_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_COUNT_125US_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_COUNT_125US_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_COUNT_125US_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_COUNT_125US_set", A, value );

    /* (0x0002f008 bits 15:0) field COUNT_125US of register PMC_FC1200_PCS_RX_REG_HIGH_BER_CFG_REG */
    fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_MSK,
                                                  FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_OFF,
                                                  value);
}

static INLINE UINT32 fc1200_pcs_rx_field_COUNT_125US_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_COUNT_125US_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_COUNT_125US_get", A, 11);
    /* (0x0002f008 bits 15:0) field COUNT_125US of register PMC_FC1200_PCS_RX_REG_HIGH_BER_CFG_REG */
    reg_value = fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_MSK) >> FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_COUNT_125US_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_rx_field_range_COUNT_125US_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_range_COUNT_125US_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_range_COUNT_125US_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_rx_field_range_COUNT_125US_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_rx_field_range_COUNT_125US_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_rx_field_range_COUNT_125US_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f008 bits 15:0) field COUNT_125US of register PMC_FC1200_PCS_RX_REG_HIGH_BER_CFG_REG */
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
        /* (0x0002f008 bits 15:0) field COUNT_125US of register PMC_FC1200_PCS_RX_REG_HIGH_BER_CFG_REG */
        fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      subfield_mask << (FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_OFF + subfield_offset),
                                                      FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 fc1200_pcs_rx_field_range_COUNT_125US_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_range_COUNT_125US_get( fc1200_pcs_rx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_range_COUNT_125US_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_rx_field_range_COUNT_125US_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_rx_field_range_COUNT_125US_get", stop_bit, 15 );
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
    /* (0x0002f008 bits 15:0) field COUNT_125US of register PMC_FC1200_PCS_RX_REG_HIGH_BER_CFG_REG */
    reg_value = fc1200_pcs_rx_reg_HIGH_BER_CFG_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    field_value = (reg_value & FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_MSK)
                  >> FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_MSK, FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_rx_field_range_COUNT_125US_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void fc1200_pcs_rx_field_PMON_UPDATE_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_PMON_UPDATE_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_PMON_UPDATE_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_PMON_UPDATE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_PMON_UPDATE_set", A, value );

    /* (0x0002f004 bits 0) field PMON_UPDATE of register PMC_FC1200_PCS_RX_REG_PMON_CNT_STAT */
    fc1200_pcs_rx_reg_PMON_CNT_STAT_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_MSK,
                                               FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_OFF,
                                               value);
}

static INLINE UINT32 fc1200_pcs_rx_field_PMON_UPDATE_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_PMON_UPDATE_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_PMON_UPDATE_get", A, 11);
    /* (0x0002f004 bits 0) field PMON_UPDATE of register PMC_FC1200_PCS_RX_REG_PMON_CNT_STAT */
    reg_value = fc1200_pcs_rx_reg_PMON_CNT_STAT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_MSK) >> FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_PMON_UPDATE_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void fc1200_pcs_rx_field_RF_E_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_RF_E_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_RF_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_RF_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_RF_E_set", A, value );

    /* (0x0002f018 bits 5) field RF_E of register PMC_FC1200_PCS_RX_REG_INT_EN_REG */
    fc1200_pcs_rx_reg_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            FC1200_PCS_RX_REG_INT_EN_REG_BIT_RF_E_MSK,
                                            FC1200_PCS_RX_REG_INT_EN_REG_BIT_RF_E_OFF,
                                            value);
}

static INLINE UINT32 fc1200_pcs_rx_field_RF_E_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_RF_E_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_RF_E_get", A, 11);
    /* (0x0002f018 bits 5) field RF_E of register PMC_FC1200_PCS_RX_REG_INT_EN_REG */
    reg_value = fc1200_pcs_rx_reg_INT_EN_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_EN_REG_BIT_RF_E_MSK) >> FC1200_PCS_RX_REG_INT_EN_REG_BIT_RF_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_RF_E_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_rx_field_LF_E_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_LF_E_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LF_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_LF_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_LF_E_set", A, value );

    /* (0x0002f018 bits 4) field LF_E of register PMC_FC1200_PCS_RX_REG_INT_EN_REG */
    fc1200_pcs_rx_reg_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            FC1200_PCS_RX_REG_INT_EN_REG_BIT_LF_E_MSK,
                                            FC1200_PCS_RX_REG_INT_EN_REG_BIT_LF_E_OFF,
                                            value);
}

static INLINE UINT32 fc1200_pcs_rx_field_LF_E_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_LF_E_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LF_E_get", A, 11);
    /* (0x0002f018 bits 4) field LF_E of register PMC_FC1200_PCS_RX_REG_INT_EN_REG */
    reg_value = fc1200_pcs_rx_reg_INT_EN_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_EN_REG_BIT_LF_E_MSK) >> FC1200_PCS_RX_REG_INT_EN_REG_BIT_LF_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_LF_E_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_rx_field_HIGH_BER_E_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_HIGH_BER_E_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_HIGH_BER_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_HIGH_BER_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_HIGH_BER_E_set", A, value );

    /* (0x0002f018 bits 2) field HIGH_BER_E of register PMC_FC1200_PCS_RX_REG_INT_EN_REG */
    fc1200_pcs_rx_reg_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            FC1200_PCS_RX_REG_INT_EN_REG_BIT_HIGH_BER_E_MSK,
                                            FC1200_PCS_RX_REG_INT_EN_REG_BIT_HIGH_BER_E_OFF,
                                            value);
}

static INLINE UINT32 fc1200_pcs_rx_field_HIGH_BER_E_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_HIGH_BER_E_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_HIGH_BER_E_get", A, 11);
    /* (0x0002f018 bits 2) field HIGH_BER_E of register PMC_FC1200_PCS_RX_REG_INT_EN_REG */
    reg_value = fc1200_pcs_rx_reg_INT_EN_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_EN_REG_BIT_HIGH_BER_E_MSK) >> FC1200_PCS_RX_REG_INT_EN_REG_BIT_HIGH_BER_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_HIGH_BER_E_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_rx_field_LOSS_SYNC_E_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_LOSS_SYNC_E_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LOSS_SYNC_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_LOSS_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_LOSS_SYNC_E_set", A, value );

    /* (0x0002f018 bits 1) field LOSS_SYNC_E of register PMC_FC1200_PCS_RX_REG_INT_EN_REG */
    fc1200_pcs_rx_reg_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOSS_SYNC_E_MSK,
                                            FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOSS_SYNC_E_OFF,
                                            value);
}

static INLINE UINT32 fc1200_pcs_rx_field_LOSS_SYNC_E_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_LOSS_SYNC_E_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LOSS_SYNC_E_get", A, 11);
    /* (0x0002f018 bits 1) field LOSS_SYNC_E of register PMC_FC1200_PCS_RX_REG_INT_EN_REG */
    reg_value = fc1200_pcs_rx_reg_INT_EN_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOSS_SYNC_E_MSK) >> FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOSS_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_LOSS_SYNC_E_get", A, value );

    return value;
}
static INLINE void fc1200_pcs_rx_field_LOS_E_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_LOS_E_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LOS_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_LOS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_LOS_E_set", A, value );

    /* (0x0002f018 bits 0) field LOS_E of register PMC_FC1200_PCS_RX_REG_INT_EN_REG */
    fc1200_pcs_rx_reg_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOS_E_MSK,
                                            FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOS_E_OFF,
                                            value);
}

static INLINE UINT32 fc1200_pcs_rx_field_LOS_E_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_LOS_E_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LOS_E_get", A, 11);
    /* (0x0002f018 bits 0) field LOS_E of register PMC_FC1200_PCS_RX_REG_INT_EN_REG */
    reg_value = fc1200_pcs_rx_reg_INT_EN_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOS_E_MSK) >> FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_LOS_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void fc1200_pcs_rx_field_RF_I_set_to_clear( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_RF_I_set_to_clear( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_RF_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_RF_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_RF_I_set_to_clear", A, value );

    /* (0x0002f01c bits 5) field RF_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    fc1200_pcs_rx_reg_INT_INDICATION_REG_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_RF_I_MSK,
                                                                    FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_RF_I_OFF,
                                                                    value);
}

static INLINE UINT32 fc1200_pcs_rx_field_RF_I_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_RF_I_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_RF_I_get", A, 11);
    /* (0x0002f01c bits 5) field RF_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    reg_value = fc1200_pcs_rx_reg_INT_INDICATION_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_RF_I_MSK) >> FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_RF_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_RF_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_RF_I_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_RF_I_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_rx_field_RF_I_poll", A, value );

    /* (0x0002f01c bits 5) field RF_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    return fc1200_pcs_rx_reg_INT_INDICATION_REG_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_RF_I_MSK,
                                                      (value<<FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_RF_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE void fc1200_pcs_rx_field_LF_I_set_to_clear( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_LF_I_set_to_clear( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LF_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_LF_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_LF_I_set_to_clear", A, value );

    /* (0x0002f01c bits 4) field LF_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    fc1200_pcs_rx_reg_INT_INDICATION_REG_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LF_I_MSK,
                                                                    FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LF_I_OFF,
                                                                    value);
}

static INLINE UINT32 fc1200_pcs_rx_field_LF_I_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_LF_I_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LF_I_get", A, 11);
    /* (0x0002f01c bits 4) field LF_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    reg_value = fc1200_pcs_rx_reg_INT_INDICATION_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LF_I_MSK) >> FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LF_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_LF_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LF_I_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LF_I_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_rx_field_LF_I_poll", A, value );

    /* (0x0002f01c bits 4) field LF_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    return fc1200_pcs_rx_reg_INT_INDICATION_REG_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LF_I_MSK,
                                                      (value<<FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LF_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE void fc1200_pcs_rx_field_HIGH_BER_I_set_to_clear( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_HIGH_BER_I_set_to_clear( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_HIGH_BER_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_HIGH_BER_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_HIGH_BER_I_set_to_clear", A, value );

    /* (0x0002f01c bits 2) field HIGH_BER_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    fc1200_pcs_rx_reg_INT_INDICATION_REG_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_HIGH_BER_I_MSK,
                                                                    FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_HIGH_BER_I_OFF,
                                                                    value);
}

static INLINE UINT32 fc1200_pcs_rx_field_HIGH_BER_I_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_HIGH_BER_I_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_HIGH_BER_I_get", A, 11);
    /* (0x0002f01c bits 2) field HIGH_BER_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    reg_value = fc1200_pcs_rx_reg_INT_INDICATION_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_HIGH_BER_I_MSK) >> FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_HIGH_BER_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_HIGH_BER_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_HIGH_BER_I_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_HIGH_BER_I_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_rx_field_HIGH_BER_I_poll", A, value );

    /* (0x0002f01c bits 2) field HIGH_BER_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    return fc1200_pcs_rx_reg_INT_INDICATION_REG_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_HIGH_BER_I_MSK,
                                                      (value<<FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_HIGH_BER_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE void fc1200_pcs_rx_field_LOSS_SYNC_I_set_to_clear( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_LOSS_SYNC_I_set_to_clear( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LOSS_SYNC_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_LOSS_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_LOSS_SYNC_I_set_to_clear", A, value );

    /* (0x0002f01c bits 1) field LOSS_SYNC_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    fc1200_pcs_rx_reg_INT_INDICATION_REG_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOSS_SYNC_I_MSK,
                                                                    FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOSS_SYNC_I_OFF,
                                                                    value);
}

static INLINE UINT32 fc1200_pcs_rx_field_LOSS_SYNC_I_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_LOSS_SYNC_I_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LOSS_SYNC_I_get", A, 11);
    /* (0x0002f01c bits 1) field LOSS_SYNC_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    reg_value = fc1200_pcs_rx_reg_INT_INDICATION_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOSS_SYNC_I_MSK) >> FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOSS_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_LOSS_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOSS_SYNC_I_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOSS_SYNC_I_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_rx_field_LOSS_SYNC_I_poll", A, value );

    /* (0x0002f01c bits 1) field LOSS_SYNC_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    return fc1200_pcs_rx_reg_INT_INDICATION_REG_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOSS_SYNC_I_MSK,
                                                      (value<<FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOSS_SYNC_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE void fc1200_pcs_rx_field_LOS_I_set_to_clear( fc1200_pcs_rx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_LOS_I_set_to_clear( fc1200_pcs_rx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LOS_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_LOS_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_LOS_I_set_to_clear", A, value );

    /* (0x0002f01c bits 0) field LOS_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    fc1200_pcs_rx_reg_INT_INDICATION_REG_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOS_I_MSK,
                                                                    FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOS_I_OFF,
                                                                    value);
}

static INLINE UINT32 fc1200_pcs_rx_field_LOS_I_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_LOS_I_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LOS_I_get", A, 11);
    /* (0x0002f01c bits 0) field LOS_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    reg_value = fc1200_pcs_rx_reg_INT_INDICATION_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOS_I_MSK) >> FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_LOS_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOS_I_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOS_I_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_rx_field_LOS_I_poll", A, value );

    /* (0x0002f01c bits 0) field LOS_I of register PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG */
    return fc1200_pcs_rx_reg_INT_INDICATION_REG_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOS_I_MSK,
                                                      (value<<FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOS_I_OFF),
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
static INLINE UINT32 fc1200_pcs_rx_field_RF_V_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_RF_V_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_RF_V_get", A, 11);
    /* (0x0002f020 bits 5) field RF_V of register PMC_FC1200_PCS_RX_REG_INT_STAT_REG */
    reg_value = fc1200_pcs_rx_reg_INT_STAT_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_STAT_REG_BIT_RF_V_MSK) >> FC1200_PCS_RX_REG_INT_STAT_REG_BIT_RF_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_RF_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_RF_V_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_RF_V_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_rx_field_RF_V_poll", A, value );

    /* (0x0002f020 bits 5) field RF_V of register PMC_FC1200_PCS_RX_REG_INT_STAT_REG */
    return fc1200_pcs_rx_reg_INT_STAT_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                FC1200_PCS_RX_REG_INT_STAT_REG_BIT_RF_V_MSK,
                                                (value<<FC1200_PCS_RX_REG_INT_STAT_REG_BIT_RF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc1200_pcs_rx_field_LF_V_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_LF_V_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LF_V_get", A, 11);
    /* (0x0002f020 bits 4) field LF_V of register PMC_FC1200_PCS_RX_REG_INT_STAT_REG */
    reg_value = fc1200_pcs_rx_reg_INT_STAT_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LF_V_MSK) >> FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LF_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_LF_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LF_V_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LF_V_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_rx_field_LF_V_poll", A, value );

    /* (0x0002f020 bits 4) field LF_V of register PMC_FC1200_PCS_RX_REG_INT_STAT_REG */
    return fc1200_pcs_rx_reg_INT_STAT_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LF_V_MSK,
                                                (value<<FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc1200_pcs_rx_field_HIGH_BER_V_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_HIGH_BER_V_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_HIGH_BER_V_get", A, 11);
    /* (0x0002f020 bits 2) field HIGH_BER_V of register PMC_FC1200_PCS_RX_REG_INT_STAT_REG */
    reg_value = fc1200_pcs_rx_reg_INT_STAT_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_STAT_REG_BIT_HIGH_BER_V_MSK) >> FC1200_PCS_RX_REG_INT_STAT_REG_BIT_HIGH_BER_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_HIGH_BER_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_HIGH_BER_V_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_HIGH_BER_V_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_rx_field_HIGH_BER_V_poll", A, value );

    /* (0x0002f020 bits 2) field HIGH_BER_V of register PMC_FC1200_PCS_RX_REG_INT_STAT_REG */
    return fc1200_pcs_rx_reg_INT_STAT_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                FC1200_PCS_RX_REG_INT_STAT_REG_BIT_HIGH_BER_V_MSK,
                                                (value<<FC1200_PCS_RX_REG_INT_STAT_REG_BIT_HIGH_BER_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc1200_pcs_rx_field_LOSS_SYNC_V_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_LOSS_SYNC_V_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LOSS_SYNC_V_get", A, 11);
    /* (0x0002f020 bits 1) field LOSS_SYNC_V of register PMC_FC1200_PCS_RX_REG_INT_STAT_REG */
    reg_value = fc1200_pcs_rx_reg_INT_STAT_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOSS_SYNC_V_MSK) >> FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOSS_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_LOSS_SYNC_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOSS_SYNC_V_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOSS_SYNC_V_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_rx_field_LOSS_SYNC_V_poll", A, value );

    /* (0x0002f020 bits 1) field LOSS_SYNC_V of register PMC_FC1200_PCS_RX_REG_INT_STAT_REG */
    return fc1200_pcs_rx_reg_INT_STAT_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOSS_SYNC_V_MSK,
                                                (value<<FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOSS_SYNC_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc1200_pcs_rx_field_LOS_V_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_LOS_V_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LOS_V_get", A, 11);
    /* (0x0002f020 bits 0) field LOS_V of register PMC_FC1200_PCS_RX_REG_INT_STAT_REG */
    reg_value = fc1200_pcs_rx_reg_INT_STAT_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOS_V_MSK) >> FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_LOS_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOS_V_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_LOS_V_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_rx_field_LOS_V_poll", A, value );

    /* (0x0002f020 bits 0) field LOS_V of register PMC_FC1200_PCS_RX_REG_INT_STAT_REG */
    return fc1200_pcs_rx_reg_INT_STAT_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOS_V_MSK,
                                                (value<<FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOS_V_OFF),
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
static INLINE UINT32 fc1200_pcs_rx_field_TIP_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_TIP_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_TIP_get", A, 11);
    /* (0x0002f004 bits 8) field TIP of register PMC_FC1200_PCS_RX_REG_PMON_CNT_STAT */
    reg_value = fc1200_pcs_rx_reg_PMON_CNT_STAT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_TIP_MSK) >> FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_TIP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_TIP_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_TIP_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc1200_pcs_rx_field_TIP_poll( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc1200_pcs_rx_field_TIP_poll", A, value );

    /* (0x0002f004 bits 8) field TIP of register PMC_FC1200_PCS_RX_REG_PMON_CNT_STAT */
    return fc1200_pcs_rx_reg_PMON_CNT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_TIP_MSK,
                                                 (value<<FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_TIP_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc1200_pcs_rx_field_REMOTE_FAULT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_REMOTE_FAULT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_REMOTE_FAULT_get", A, 11);
    /* (0x0002f014 bits 1) field REMOTE_FAULT of register PMC_FC1200_PCS_RX_REG_STAT_REG */
    reg_value = fc1200_pcs_rx_reg_STAT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_RX_REG_STAT_REG_BIT_REMOTE_FAULT_MSK) >> FC1200_PCS_RX_REG_STAT_REG_BIT_REMOTE_FAULT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_REMOTE_FAULT_get", A, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_rx_field_LOCAL_FAULT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_LOCAL_FAULT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_LOCAL_FAULT_get", A, 11);
    /* (0x0002f014 bits 0) field LOCAL_FAULT of register PMC_FC1200_PCS_RX_REG_STAT_REG */
    reg_value = fc1200_pcs_rx_reg_STAT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_RX_REG_STAT_REG_BIT_LOCAL_FAULT_MSK) >> FC1200_PCS_RX_REG_STAT_REG_BIT_LOCAL_FAULT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_LOCAL_FAULT_get", A, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_rx_field_HIGH_BER_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_HIGH_BER_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_HIGH_BER_ERR_CNT_get", A, 11);
    /* (0x0002f024 bits 23:16) field HIGH_BER_ERR_CNT of register PMC_FC1200_PCS_RX_REG_ERR_CNTS */
    reg_value = fc1200_pcs_rx_reg_ERR_CNTS_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_RX_REG_ERR_CNTS_BIT_HIGH_BER_ERR_CNT_MSK) >> FC1200_PCS_RX_REG_ERR_CNTS_BIT_HIGH_BER_ERR_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_HIGH_BER_ERR_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_rx_field_range_HIGH_BER_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_range_HIGH_BER_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_range_HIGH_BER_ERR_CNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_rx_field_range_HIGH_BER_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_rx_field_range_HIGH_BER_ERR_CNT_get", stop_bit, 7 );
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
    /* (0x0002f024 bits 23:16) field HIGH_BER_ERR_CNT of register PMC_FC1200_PCS_RX_REG_ERR_CNTS */
    reg_value = fc1200_pcs_rx_reg_ERR_CNTS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC1200_PCS_RX_REG_ERR_CNTS_BIT_HIGH_BER_ERR_CNT_MSK)
                  >> FC1200_PCS_RX_REG_ERR_CNTS_BIT_HIGH_BER_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_RX_REG_ERR_CNTS_BIT_HIGH_BER_ERR_CNT_MSK, FC1200_PCS_RX_REG_ERR_CNTS_BIT_HIGH_BER_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_rx_field_range_HIGH_BER_ERR_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_rx_field_PRBS31_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_PRBS31_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_PRBS31_ERR_CNT_get", A, 11);
    /* (0x0002f024 bits 15:0) field PRBS31_ERR_CNT of register PMC_FC1200_PCS_RX_REG_ERR_CNTS */
    reg_value = fc1200_pcs_rx_reg_ERR_CNTS_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_RX_REG_ERR_CNTS_BIT_PRBS31_ERR_CNT_MSK) >> FC1200_PCS_RX_REG_ERR_CNTS_BIT_PRBS31_ERR_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_PRBS31_ERR_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_rx_field_range_PRBS31_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_range_PRBS31_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_range_PRBS31_ERR_CNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_rx_field_range_PRBS31_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_rx_field_range_PRBS31_ERR_CNT_get", stop_bit, 15 );
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
    /* (0x0002f024 bits 15:0) field PRBS31_ERR_CNT of register PMC_FC1200_PCS_RX_REG_ERR_CNTS */
    reg_value = fc1200_pcs_rx_reg_ERR_CNTS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC1200_PCS_RX_REG_ERR_CNTS_BIT_PRBS31_ERR_CNT_MSK)
                  >> FC1200_PCS_RX_REG_ERR_CNTS_BIT_PRBS31_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_RX_REG_ERR_CNTS_BIT_PRBS31_ERR_CNT_MSK, FC1200_PCS_RX_REG_ERR_CNTS_BIT_PRBS31_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_rx_field_range_PRBS31_ERR_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_rx_field_BLK_TYPE_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_BLK_TYPE_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_BLK_TYPE_ERR_CNT_get", A, 11);
    /* (0x0002f028 bits 31:16) field BLK_TYPE_ERR_CNT of register PMC_FC1200_PCS_RX_REG_BLOCK_ERR_CNTS */
    reg_value = fc1200_pcs_rx_reg_BLOCK_ERR_CNTS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_BLK_TYPE_ERR_CNT_MSK) >> FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_BLK_TYPE_ERR_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_BLK_TYPE_ERR_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_rx_field_range_BLK_TYPE_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_range_BLK_TYPE_ERR_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_range_BLK_TYPE_ERR_CNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_rx_field_range_BLK_TYPE_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_rx_field_range_BLK_TYPE_ERR_CNT_get", stop_bit, 15 );
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
    /* (0x0002f028 bits 31:16) field BLK_TYPE_ERR_CNT of register PMC_FC1200_PCS_RX_REG_BLOCK_ERR_CNTS */
    reg_value = fc1200_pcs_rx_reg_BLOCK_ERR_CNTS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_BLK_TYPE_ERR_CNT_MSK)
                  >> FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_BLK_TYPE_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_BLK_TYPE_ERR_CNT_MSK, FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_BLK_TYPE_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_rx_field_range_BLK_TYPE_ERR_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_rx_field_INVLD_BLK_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_INVLD_BLK_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_INVLD_BLK_CNT_get", A, 11);
    /* (0x0002f028 bits 15:0) field INVLD_BLK_CNT of register PMC_FC1200_PCS_RX_REG_BLOCK_ERR_CNTS */
    reg_value = fc1200_pcs_rx_reg_BLOCK_ERR_CNTS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_INVLD_BLK_CNT_MSK) >> FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_INVLD_BLK_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_INVLD_BLK_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_rx_field_range_INVLD_BLK_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_range_INVLD_BLK_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_range_INVLD_BLK_CNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_rx_field_range_INVLD_BLK_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_rx_field_range_INVLD_BLK_CNT_get", stop_bit, 15 );
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
    /* (0x0002f028 bits 15:0) field INVLD_BLK_CNT of register PMC_FC1200_PCS_RX_REG_BLOCK_ERR_CNTS */
    reg_value = fc1200_pcs_rx_reg_BLOCK_ERR_CNTS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_INVLD_BLK_CNT_MSK)
                  >> FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_INVLD_BLK_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_INVLD_BLK_CNT_MSK, FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_INVLD_BLK_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_rx_field_range_INVLD_BLK_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc1200_pcs_rx_lfield_DATA_BYTE_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_lfield_DATA_BYTE_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_lfield_DATA_BYTE_CNT_get", A, 11);
    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0002f02c bits 31:0) bits 0:31 use field DATA_BYTE_CNT of register PMC_FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB */
    reg_value = fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_LSB_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_MSK) >> FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_OFF;
    value[0] |= field_value;

    /* (0x0002f030 bits 7:0) bits 32:39 use field DATA_BYTE_CNT of register PMC_FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB */
    reg_value = fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_MSK) >> FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> A=%d 0x%08x 0x%08x", "fc1200_pcs_rx_lfield_DATA_BYTE_CNT_get", A, value[1] , value[0] );

}
static INLINE UINT32 fc1200_pcs_rx_lfield_range_DATA_BYTE_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_lfield_range_DATA_BYTE_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_lfield_range_DATA_BYTE_CNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_rx_lfield_range_DATA_BYTE_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_rx_lfield_range_DATA_BYTE_CNT_get", stop_bit, 39 );
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
    /* (0x0002f02c bits 31:0) bits 0:31 use field DATA_BYTE_CNT of register PMC_FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB */
        reg_value = fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_LSB_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_MSK)
                      >> FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_MSK, FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_OFF, field_value );
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
    /* (0x0002f030 bits 7:0) bits 32:39 use field DATA_BYTE_CNT of register PMC_FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB */
        reg_value = fc1200_pcs_rx_reg_RXD_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_MSK)
                      >> FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_MSK, FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_rx_lfield_range_DATA_BYTE_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_rx_field_ORDERED_SET_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_ORDERED_SET_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_ORDERED_SET_CNT_get", A, 11);
    /* (0x0002f034 bits 31:0) field ORDERED_SET_CNT of register PMC_FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT */
    reg_value = fc1200_pcs_rx_reg_RXD_ORDERED_SET_CNT_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_MSK) >> FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_ORDERED_SET_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc1200_pcs_rx_field_range_ORDERED_SET_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_range_ORDERED_SET_CNT_get( fc1200_pcs_rx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_range_ORDERED_SET_CNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc1200_pcs_rx_field_range_ORDERED_SET_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc1200_pcs_rx_field_range_ORDERED_SET_CNT_get", stop_bit, 31 );
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
    /* (0x0002f034 bits 31:0) field ORDERED_SET_CNT of register PMC_FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT */
    reg_value = fc1200_pcs_rx_reg_RXD_ORDERED_SET_CNT_read( b_ptr,
                                                            h_ptr,
                                                            A);
    field_value = (reg_value & FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_MSK)
                  >> FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_MSK, FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc1200_pcs_rx_field_range_ORDERED_SET_CNT_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset sw_reset
 * ==================================================================================
 */
static INLINE void fc1200_pcs_rx_field_SW_RESET_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc1200_pcs_rx_field_SW_RESET_set( fc1200_pcs_rx_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_SW_RESET_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc1200_pcs_rx_field_SW_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc1200_pcs_rx_field_SW_RESET_set", A, value );

    /* (0x0002f000 bits 0) field SW_RESET of register PMC_FC1200_PCS_RX_REG_CFG_REG */
    fc1200_pcs_rx_reg_CFG_REG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         FC1200_PCS_RX_REG_CFG_REG_BIT_SW_RESET_MSK,
                                         FC1200_PCS_RX_REG_CFG_REG_BIT_SW_RESET_OFF,
                                         value);
}

static INLINE UINT32 fc1200_pcs_rx_field_SW_RESET_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc1200_pcs_rx_field_SW_RESET_get( fc1200_pcs_rx_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc1200_pcs_rx_field_SW_RESET_get", A, 11);
    /* (0x0002f000 bits 0) field SW_RESET of register PMC_FC1200_PCS_RX_REG_CFG_REG */
    reg_value = fc1200_pcs_rx_reg_CFG_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC1200_PCS_RX_REG_CFG_REG_BIT_SW_RESET_MSK) >> FC1200_PCS_RX_REG_CFG_REG_BIT_SW_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc1200_pcs_rx_field_SW_RESET_get", A, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC1200_PCS_RX_IO_INLINE_H */
