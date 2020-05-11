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
 *     and register accessor functions for the otu_frm_rx block
 *****************************************************************************/
#ifndef _OTU_FRM_RX_IO_INLINE_H
#define _OTU_FRM_RX_IO_INLINE_H

#include "pmc_sys.h"
#include "pmc_types.h"
#include "otu_frm_rx_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OTU_FRM_RX_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for otu_frm_rx
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
    otu_frm_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} otu_frm_rx_buffer_t;
static INLINE void otu_frm_rx_buffer_init( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_buffer_init( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "otu_frm_rx_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void otu_frm_rx_buffer_flush( otu_frm_rx_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_buffer_flush( otu_frm_rx_buffer_t *b_ptr )
{
    IOLOG( "otu_frm_rx_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 otu_frm_rx_reg_read( otu_frm_rx_buffer_t *b_ptr,
                                          otu_frm_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_read( otu_frm_rx_buffer_t *b_ptr,
                                          otu_frm_handle_t *h_ptr,
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
static INLINE void otu_frm_rx_reg_write( otu_frm_rx_buffer_t *b_ptr,
                                         otu_frm_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_write( otu_frm_rx_buffer_t *b_ptr,
                                         otu_frm_handle_t *h_ptr,
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

static INLINE void otu_frm_rx_field_set( otu_frm_rx_buffer_t *b_ptr,
                                         otu_frm_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 mask,
                                         UINT32 unused_mask,
                                         UINT32 ofs,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_set( otu_frm_rx_buffer_t *b_ptr,
                                         otu_frm_handle_t *h_ptr,
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

static INLINE void otu_frm_rx_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
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

static INLINE void otu_frm_rx_burst_read( otu_frm_rx_buffer_t *b_ptr,
                                          otu_frm_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_burst_read( otu_frm_rx_buffer_t *b_ptr,
                                          otu_frm_handle_t *h_ptr,
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

static INLINE void otu_frm_rx_burst_write( otu_frm_rx_buffer_t *b_ptr,
                                           otu_frm_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_burst_write( otu_frm_rx_buffer_t *b_ptr,
                                           otu_frm_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_poll( otu_frm_rx_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                    UINT32 max_count,
                                                    UINT32 *num_failed_polls,
                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_poll( otu_frm_rx_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
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
 *  register access functions for otu_frm_rx
 * ==================================================================================
 */

static INLINE void otu_frm_rx_reg_OTU_SK_CFG_write( otu_frm_rx_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_CFG_write( otu_frm_rx_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_CFG_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_RX_REG_OTU_SK_CFG,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_CFG_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_CFG_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_CFG_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_RX_REG_OTU_SK_CFG,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_CFG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_CFG_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_CFG_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_CFG);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_CFG_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_write( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_write( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR(N),
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_field_set", N, mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR(N),
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_SM_CTL_array_write( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SM_CTL_array_write( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_SM_CTL_array_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL(N),
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set", N, mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL(N),
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_rx_reg_OTU_SK_SM_CTL_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_PASSTHRU_array_write( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_PASSTHRU_array_write( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_PASSTHRU_array_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU(N),
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_PASSTHRU_array_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_PASSTHRU_array_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_PASSTHRU_array_field_set", N, mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU(N),
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_PASSTHRU_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_PASSTHRU_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_rx_reg_OTU_SK_PASSTHRU_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_AIS_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_AIS_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_AIS_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_AIS_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_AIS_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_AIS_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_AIS_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_AIS_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_AIS_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_SSD_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_SSD_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_SSD_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_SSD_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_SSD_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_SSD_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSD_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSD_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_SSD_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_SSF_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_SSF_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_SSF_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_SSF_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_SSF_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_SSF_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSF_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSF_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_SSF_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_TSF_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_TSF_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_TSF_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_TSF_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_TSF_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_TSF_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSF_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSF_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_TSF_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_TSD_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_TSD_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_TSD_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_TSD_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_TSD_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_TSD_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSD_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSD_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_TSD_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_BIAE_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BIAE_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_BIAE_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_BIAE_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BIAE_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_BIAE_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BIAE_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BIAE_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_BIAE_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_BEI_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BEI_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_BEI_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_BEI_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BEI_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_BEI_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BEI_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BEI_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_BEI_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_BDI_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BDI_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_BDI_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_BDI_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BDI_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_BDI_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BDI_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BDI_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_BDI_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_IAE_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_IAE_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_D_IAE_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_IAE_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_IAE_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_D_IAE_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_IAE_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_IAE_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_IAE_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_TIM_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_TIM_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_D_TIM_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_TIM_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_TIM_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_D_TIM_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_TIM_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_TIM_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_TIM_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_DEG_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_DEG_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_D_DEG_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_DEG_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_DEG_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_D_DEG_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_DEG_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_DEG_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_DEG_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_BDI_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_BDI_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_D_BDI_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_BDI_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_BDI_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_D_BDI_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BDI_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BDI_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_BDI_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_BIAE_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_BIAE_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_D_BIAE_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_BIAE_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_BIAE_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_D_BIAE_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BIAE_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BIAE_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_BIAE_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_CI_SSF_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_CI_SSF_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_CI_SSF_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_CI_SSF_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_CI_SSF_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_CI_SSF_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_CI_SSF_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_CI_SSF_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_CI_SSF_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_write( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_field_set", mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E,
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_AIS_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_AIS_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_AIS_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_AIS_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_AIS_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_AIS_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_AIS_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_AIS_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_AIS_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_AIS_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_AIS_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_AIS_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_A_SSD_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_SSD_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_SSD_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_SSD_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_SSD_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_SSD_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSD_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSD_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_SSD_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_SSD_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_A_SSF_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_SSF_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_SSF_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_SSF_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_SSF_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_SSF_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSF_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSF_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_SSF_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_SSF_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_A_TSF_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_TSF_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_TSF_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_TSF_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_TSF_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_TSF_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSF_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSF_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_TSF_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_TSF_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_A_TSD_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_TSD_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_TSD_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_TSD_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_TSD_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_TSD_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSD_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSD_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_TSD_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_TSD_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_A_BIAE_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BIAE_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_BIAE_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_BIAE_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BIAE_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_BIAE_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BIAE_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BIAE_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_BIAE_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_BIAE_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_A_BEI_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BEI_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_BEI_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_BEI_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BEI_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_BEI_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BEI_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BEI_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_BEI_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BEI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BEI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_BEI_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_A_BDI_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BDI_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_A_BDI_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_A_BDI_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_A_BDI_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_A_BDI_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BDI_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BDI_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_BDI_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_BDI_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_D_IAE_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_IAE_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_D_IAE_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_IAE_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_IAE_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_D_IAE_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_IAE_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_IAE_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_IAE_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_IAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_IAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_D_IAE_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_D_TIM_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_TIM_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_D_TIM_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_TIM_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_TIM_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_D_TIM_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_TIM_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_TIM_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_TIM_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_TIM_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_TIM_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_D_TIM_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_D_DEG_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_DEG_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_D_DEG_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_DEG_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_DEG_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_D_DEG_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_DEG_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_DEG_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_DEG_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_DEG_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_DEG_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_D_DEG_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_D_BDI_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_BDI_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_D_BDI_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_BDI_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_BDI_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_D_BDI_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BDI_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BDI_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_BDI_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_D_BDI_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_D_BIAE_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_BIAE_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_D_BIAE_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_D_BIAE_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_D_BIAE_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_D_BIAE_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BIAE_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BIAE_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_BIAE_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_D_BIAE_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_CI_SSF_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_CI_SSF_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_CI_SSF_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_CI_SSF_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_CI_SSF_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_CI_SSF_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_CI_SSF_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_CI_SSF_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_CI_SSF_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_CI_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_CI_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_CI_SSF_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_write( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_write", value );
    otu_frm_rx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I,
                          value);
}

static INLINE void otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                                  otu_frm_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_action_on_write_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                                  otu_frm_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_rx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_AIS_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_AIS_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_AIS_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_AIS_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_AIS_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_AIS_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSD_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSD_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_SSD_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_SSD_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSF_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_SSF_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_SSF_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_SSF_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSF_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSF_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_TSF_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_TSF_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSD_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_TSD_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_TSD_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_TSD_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BIAE_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BIAE_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_BIAE_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_BIAE_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BEI_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BEI_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_BEI_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BEI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BEI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_BEI_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BDI_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_A_BDI_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_A_BDI_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_A_BDI_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_IAE_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_IAE_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_IAE_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_IAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_IAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_D_IAE_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_TIM_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_TIM_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_TIM_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_TIM_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_TIM_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_D_TIM_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_DEG_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_DEG_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_DEG_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_DEG_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_DEG_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_D_DEG_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BDI_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BDI_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_BDI_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_D_BDI_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BIAE_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_D_BIAE_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_D_BIAE_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_D_BIAE_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_CI_SSF_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_CI_SSF_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_CI_SSF_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_CI_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_CI_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_CI_SSF_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SHADOW_XFER_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SHADOW_XFER_V_read( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_rx_reg_OTU_SK_SHADOW_XFER_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_SHADOW_XFER_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_SHADOW_XFER_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_rx_reg_OTU_SK_SHADOW_XFER_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_rx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_OH_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_OH_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_rx_reg_OTU_SK_SM_OH_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_OH2_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_OH2_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_OH2(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_rx_reg_OTU_SK_OH2_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_write( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_write( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 16)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_write", ofs, len, 16 );
    IOLOG( "%s of %d words N=%d ofs=%d len=%d", "otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_write", 16, N, ofs, len);
    otu_frm_rx_burst_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI(N,ofs),
                            len,
                            value);

}

static INLINE void otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 M,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_field_set( otu_frm_rx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 M,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_field_set", N, mask, ofs, value );
    otu_frm_rx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI(N,M),
                          mask,
                          PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE void otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_read( otu_frm_rx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_read( otu_frm_rx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 16)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_read", ofs, len, 16 );
    otu_frm_rx_burst_read( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI(N,ofs),
                           len,
                           value);

    IOLOG( "%s of %d words N=%d ofs=%d len=%d", "otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_read", 16, N, ofs, len);
}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 M ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 M )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI(N,M));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_rx_reg_OTU_SK_SM_AC_TTI_array_burst_read( otu_frm_rx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_reg_OTU_SK_SM_AC_TTI_array_burst_read( otu_frm_rx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 16)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "otu_frm_rx_reg_OTU_SK_SM_AC_TTI_array_burst_read", ofs, len, 16 );
    otu_frm_rx_burst_read( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI(N,ofs),
                           len,
                           value);

    IOLOG( "%s of %d words N=%d ofs=%d len=%d", "otu_frm_rx_reg_OTU_SK_SM_AC_TTI_array_burst_read", 16, N, ofs, len);
}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_AC_TTI_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 M ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_AC_TTI_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 M )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI(N,M));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_rx_reg_OTU_SK_SM_AC_TTI_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_PMON_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_PMON_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_rx_reg_OTU_SK_SM_PMON_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_FE_PMON_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_reg_OTU_SK_SM_FE_PMON_array_read( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_rx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_rx_reg_OTU_SK_SM_FE_PMON_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void otu_frm_rx_field_OTU_SK_AC_TTI_set( otu_frm_rx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_AC_TTI_set( otu_frm_rx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_AC_TTI_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_AC_TTI_set", value );

    /* (0x00002f00 bits 2) field OTU_SK_AC_TTI_FIELD of register PMC_OTU_FRM_RX_REG_OTU_SK_CFG */
    otu_frm_rx_reg_OTU_SK_CFG_field_set( b_ptr,
                                         h_ptr,
                                         OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_AC_TTI_FIELD_MSK,
                                         OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_AC_TTI_FIELD_OFF,
                                         value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_AC_TTI_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_AC_TTI_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f00 bits 2) field OTU_SK_AC_TTI_FIELD of register PMC_OTU_FRM_RX_REG_OTU_SK_CFG */
    reg_value = otu_frm_rx_reg_OTU_SK_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_AC_TTI_FIELD_MSK) >> OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_AC_TTI_FIELD_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_AC_TTI_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_TTI_LOCK_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_TTI_LOCK_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_TTI_LOCK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_TTI_LOCK_set", value );

    /* (0x00002f00 bits 1) field OTU_SK_TTI_LOCK of register PMC_OTU_FRM_RX_REG_OTU_SK_CFG */
    otu_frm_rx_reg_OTU_SK_CFG_field_set( b_ptr,
                                         h_ptr,
                                         OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_TTI_LOCK_MSK,
                                         OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_TTI_LOCK_OFF,
                                         value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_TTI_LOCK_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_TTI_LOCK_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f00 bits 1) field OTU_SK_TTI_LOCK of register PMC_OTU_FRM_RX_REG_OTU_SK_CFG */
    reg_value = otu_frm_rx_reg_OTU_SK_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_TTI_LOCK_MSK) >> OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_TTI_LOCK_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_TTI_LOCK_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_LOCK_set( otu_frm_rx_buffer_t *b_ptr,
                                                     otu_frm_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_LOCK_set( otu_frm_rx_buffer_t *b_ptr,
                                                     otu_frm_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_LOCK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_LOCK_set", value );

    /* (0x00002f00 bits 0) field OTU_SK_LOCK of register PMC_OTU_FRM_RX_REG_OTU_SK_CFG */
    otu_frm_rx_reg_OTU_SK_CFG_field_set( b_ptr,
                                         h_ptr,
                                         OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_LOCK_MSK,
                                         OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_LOCK_OFF,
                                         value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_LOCK_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_LOCK_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f00 bits 0) field OTU_SK_LOCK of register PMC_OTU_FRM_RX_REG_OTU_SK_CFG */
    reg_value = otu_frm_rx_reg_OTU_SK_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_LOCK_MSK) >> OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_LOCK_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_LOCK_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void otu_frm_rx_field_OTU_SK_SM_DEG_M_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_SM_DEG_M_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_DEG_M_set", N, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_SM_DEG_M_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_DEG_M_set", N, value );

    /* ((0x00002040 + (N) * 0x100) bits 23:20) bits 0:3 use field OTU_SK_SM_DEG_M of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_MSK,
                                                      OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_DEG_M_get( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_DEG_M_get( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_DEG_M_get", N, 11);
    /* ((0x00002040 + (N) * 0x100) bits 23:20) bits 0:3 use field OTU_SK_SM_DEG_M of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_DEG_M_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_SM_DEG_M_set( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_SM_DEG_M_set( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_M_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_M_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_M_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_SM_DEG_M_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00002040 + (N) * 0x100) bits 23:20) bits 0:3 use field OTU_SK_SM_DEG_M of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR index N=0..11 */
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
        /* ((0x00002040 + (N) * 0x100) bits 23:20) bits 0:3 use field OTU_SK_SM_DEG_M of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR index N=0..11 */
        otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          subfield_mask << (OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_OFF + subfield_offset),
                                                          OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_DEG_M_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_DEG_M_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_M_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_M_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_M_get", stop_bit, 3 );
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
    /* ((0x00002040 + (N) * 0x100) bits 23:20) bits 0:3 use field OTU_SK_SM_DEG_M of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_MSK, OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_SM_DEG_M_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_SM_DEG_THR_set( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_SM_DEG_THR_set( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_DEG_THR_set", N, 11);
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_SM_DEG_THR_set", value, 1048575);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_DEG_THR_set", N, value );

    /* ((0x00002040 + (N) * 0x100) bits 19:0) bits 0:19 use field OTU_SK_SM_DEG_THR of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_MSK,
                                                      OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_DEG_THR_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_DEG_THR_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_DEG_THR_get", N, 11);
    /* ((0x00002040 + (N) * 0x100) bits 19:0) bits 0:19 use field OTU_SK_SM_DEG_THR of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_DEG_THR_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_set", stop_bit, 19 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00002040 + (N) * 0x100) bits 19:0) bits 0:19 use field OTU_SK_SM_DEG_THR of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR index N=0..11 */
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
        /* ((0x00002040 + (N) * 0x100) bits 19:0) bits 0:19 use field OTU_SK_SM_DEG_THR of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR index N=0..11 */
        otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          subfield_mask << (OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_OFF + subfield_offset),
                                                          OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_get", stop_bit, 19 );
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
    /* ((0x00002040 + (N) * 0x100) bits 19:0) bits 0:19 use field OTU_SK_SM_DEG_THR of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_MSK, OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 22) field OTU_SK_BDI_DIS_FROM_TTI of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_TTI_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_TTI_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 22) field OTU_SK_BDI_DIS_FROM_TTI of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_TTI_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_TTI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 21) field OTU_SK_BDI_DIS_FROM_SSF of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_SSF_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_SSF_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 21) field OTU_SK_BDI_DIS_FROM_SSF of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_SSF_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_SSF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_BEI_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_BEI_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_BEI_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_BEI_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_BEI_DIS_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 20) field OTU_SK_BEI_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BEI_DIS_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BEI_DIS_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_BEI_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_BEI_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_BEI_DIS_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 20) field OTU_SK_BEI_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BEI_DIS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BEI_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_BEI_DIS_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_BIAE_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_BIAE_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_BIAE_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_BIAE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_BIAE_DIS_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 19) field OTU_SK_BIAE_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BIAE_DIS_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BIAE_DIS_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_BIAE_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_BIAE_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_BIAE_DIS_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 19) field OTU_SK_BIAE_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BIAE_DIS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BIAE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_BIAE_DIS_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_TSD_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_TSD_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_TSD_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_TSD_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_TSD_DIS_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 18) field OTU_SK_TSD_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSD_DIS_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSD_DIS_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_TSD_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_TSD_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_TSD_DIS_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 18) field OTU_SK_TSD_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSD_DIS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSD_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_TSD_DIS_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 17) field OTU_SK_TSF_DIS_FROM_SSF of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSF_DIS_FROM_SSF_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSF_DIS_FROM_SSF_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 17) field OTU_SK_TSF_DIS_FROM_SSF of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSF_DIS_FROM_SSF_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSF_DIS_FROM_SSF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 16) field OTU_SK_TIM_ACT_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_ACT_DIS_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_ACT_DIS_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 16) field OTU_SK_TIM_ACT_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_ACT_DIS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_ACT_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 14) field OTU_SK_ADAPT_SSD_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSD_DIS_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSD_DIS_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 14) field OTU_SK_ADAPT_SSD_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSD_DIS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSD_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 13) field OTU_SK_ADAPT_SSF_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSF_DIS_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSF_DIS_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 13) field OTU_SK_ADAPT_SSF_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSF_DIS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSF_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 12) field OTU_SK_ADAPT_AIS_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_AIS_DIS_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_AIS_DIS_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 12) field OTU_SK_ADAPT_AIS_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_AIS_DIS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_AIS_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_MI_LCK_set( otu_frm_rx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_MI_LCK_set( otu_frm_rx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_MI_LCK_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_MI_LCK_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_MI_LCK_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 8) field OTU_SK_MI_LCK of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MI_LCK_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MI_LCK_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_MI_LCK_get( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_MI_LCK_get( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_MI_LCK_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 8) field OTU_SK_MI_LCK of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MI_LCK_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MI_LCK_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_MI_LCK_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_SSF_MI_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_SSF_MI_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SSF_MI_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_SSF_MI_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SSF_MI_DIS_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 7) field OTU_SK_SSF_MI_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSF_MI_DIS_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSF_MI_DIS_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_SSF_MI_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SSF_MI_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SSF_MI_DIS_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 7) field OTU_SK_SSF_MI_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSF_MI_DIS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSF_MI_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SSF_MI_DIS_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_AIS_MI_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_AIS_MI_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_AIS_MI_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_AIS_MI_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_AIS_MI_DIS_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 6) field OTU_SK_AIS_MI_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_AIS_MI_DIS_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_AIS_MI_DIS_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_AIS_MI_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_AIS_MI_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_AIS_MI_DIS_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 6) field OTU_SK_AIS_MI_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_AIS_MI_DIS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_AIS_MI_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_AIS_MI_DIS_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_SSD_MI_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_SSD_MI_DIS_set( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SSD_MI_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_SSD_MI_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SSD_MI_DIS_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 5) field OTU_SK_SSD_MI_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSD_MI_DIS_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSD_MI_DIS_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_SSD_MI_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SSD_MI_DIS_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SSD_MI_DIS_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 5) field OTU_SK_SSD_MI_DIS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSD_MI_DIS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSD_MI_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SSD_MI_DIS_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_TIM_DET_MO_set( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_TIM_DET_MO_set( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_TIM_DET_MO_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_TIM_DET_MO_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_TIM_DET_MO_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 3:2) field OTU_SK_TIM_DET_MO of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_TIM_DET_MO_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_TIM_DET_MO_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_TIM_DET_MO_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 3:2) field OTU_SK_TIM_DET_MO of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_TIM_DET_MO_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_set( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00002044 + (N) * 0x100) bits 3:2) field OTU_SK_TIM_DET_MO of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
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
        /* ((0x00002044 + (N) * 0x100) bits 3:2) field OTU_SK_TIM_DET_MO of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
        otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_OFF + subfield_offset),
                                                      OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_get", stop_bit, 1 );
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
    /* ((0x00002044 + (N) * 0x100) bits 3:2) field OTU_SK_TIM_DET_MO of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_MSK, OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_MAINT_SIG_set( otu_frm_rx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_MAINT_SIG_set( otu_frm_rx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_MAINT_SIG_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_MAINT_SIG_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_MAINT_SIG_set", N, value );

    /* ((0x00002044 + (N) * 0x100) bits 1:0) field OTU_SK_MAINT_SIG of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_MSK,
                                                  OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_OFF,
                                                  value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_MAINT_SIG_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_MAINT_SIG_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_MAINT_SIG_get", N, 11);
    /* ((0x00002044 + (N) * 0x100) bits 1:0) field OTU_SK_MAINT_SIG of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_MAINT_SIG_get", N, value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_MAINT_SIG_set( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_MAINT_SIG_set( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_MAINT_SIG_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_MAINT_SIG_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_MAINT_SIG_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_MAINT_SIG_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00002044 + (N) * 0x100) bits 1:0) field OTU_SK_MAINT_SIG of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
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
        /* ((0x00002044 + (N) * 0x100) bits 1:0) field OTU_SK_MAINT_SIG of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
        otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_OFF + subfield_offset),
                                                      OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_MAINT_SIG_get( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_MAINT_SIG_get( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_MAINT_SIG_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_MAINT_SIG_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_MAINT_SIG_get", stop_bit, 1 );
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
    /* ((0x00002044 + (N) * 0x100) bits 1:0) field OTU_SK_MAINT_SIG of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_CTL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_MSK, OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_MAINT_SIG_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_PASSTHRU_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_PASSTHRU_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_PASSTHRU_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_PASSTHRU_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_PASSTHRU_set", N, value );

    /* ((0x00002048 + (N) * 0x100) bits 0) field OTU_SK_PASSTHRU of register PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU index N=0..11 */
    otu_frm_rx_reg_OTU_SK_PASSTHRU_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_RX_REG_OTU_SK_PASSTHRU_BIT_OTU_SK_PASSTHRU_MSK,
                                                    OTU_FRM_RX_REG_OTU_SK_PASSTHRU_BIT_OTU_SK_PASSTHRU_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_PASSTHRU_get( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_PASSTHRU_get( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_PASSTHRU_get", N, 11);
    /* ((0x00002048 + (N) * 0x100) bits 0) field OTU_SK_PASSTHRU of register PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_PASSTHRU_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_PASSTHRU_BIT_OTU_SK_PASSTHRU_MSK) >> OTU_FRM_RX_REG_OTU_SK_PASSTHRU_BIT_OTU_SK_PASSTHRU_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_PASSTHRU_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void otu_frm_rx_field_OTU_SK_A_AIS_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_AIS_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_AIS_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_AIS_E_set", value );

    /* (0x00002f40 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_E */
    otu_frm_rx_reg_OTU_SK_A_AIS_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_MSK,
                                             OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_AIS_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_AIS_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f40 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_AIS_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_AIS_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_AIS_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_AIS_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_AIS_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_AIS_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_AIS_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f40 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_E */
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
        /* (0x00002f40 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_E */
        otu_frm_rx_reg_OTU_SK_A_AIS_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_OFF + subfield_offset),
                                                 OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_AIS_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_AIS_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_AIS_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_AIS_E_get", stop_bit, 11 );
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
    /* (0x00002f40 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_AIS_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_MSK, OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_AIS_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_A_SSD_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_SSD_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSD_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSD_E_set", value );

    /* (0x00002f44 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_E */
    otu_frm_rx_reg_OTU_SK_A_SSD_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_MSK,
                                             OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSD_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSD_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f44 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSD_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSD_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_SSD_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_SSD_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSD_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSD_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSD_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f44 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_E */
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
        /* (0x00002f44 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_E */
        otu_frm_rx_reg_OTU_SK_A_SSD_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_OFF + subfield_offset),
                                                 OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSD_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSD_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSD_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSD_E_get", stop_bit, 11 );
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
    /* (0x00002f44 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSD_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_MSK, OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSD_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_A_SSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_SSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSF_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSF_E_set", value );

    /* (0x00002f48 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_E */
    otu_frm_rx_reg_OTU_SK_A_SSF_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_MSK,
                                             OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSF_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSF_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f48 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSF_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSF_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_SSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_SSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSF_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSF_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSF_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f48 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_E */
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
        /* (0x00002f48 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_E */
        otu_frm_rx_reg_OTU_SK_A_SSF_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_OFF + subfield_offset),
                                                 OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSF_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSF_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSF_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSF_E_get", stop_bit, 11 );
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
    /* (0x00002f48 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSF_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_MSK, OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSF_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_A_TSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_TSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSF_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSF_E_set", value );

    /* (0x00002f4c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_E */
    otu_frm_rx_reg_OTU_SK_A_TSF_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_MSK,
                                             OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSF_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSF_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f4c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSF_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSF_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_TSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_TSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSF_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSF_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSF_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f4c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_E */
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
        /* (0x00002f4c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_E */
        otu_frm_rx_reg_OTU_SK_A_TSF_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_OFF + subfield_offset),
                                                 OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSF_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSF_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSF_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSF_E_get", stop_bit, 11 );
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
    /* (0x00002f4c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSF_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_MSK, OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSF_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_A_TSD_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_TSD_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSD_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSD_E_set", value );

    /* (0x00002f50 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_E */
    otu_frm_rx_reg_OTU_SK_A_TSD_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_MSK,
                                             OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSD_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSD_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f50 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSD_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSD_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_TSD_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_TSD_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSD_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSD_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSD_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f50 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_E */
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
        /* (0x00002f50 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_E */
        otu_frm_rx_reg_OTU_SK_A_TSD_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_OFF + subfield_offset),
                                                 OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSD_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSD_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSD_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSD_E_get", stop_bit, 11 );
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
    /* (0x00002f50 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSD_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_MSK, OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSD_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_A_BIAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_BIAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_BIAE_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_BIAE_E_set", value );

    /* (0x00002f54 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_E */
    otu_frm_rx_reg_OTU_SK_A_BIAE_E_field_set( b_ptr,
                                              h_ptr,
                                              OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_MSK,
                                              OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_OFF,
                                              value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BIAE_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BIAE_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f54 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BIAE_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_BIAE_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BIAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BIAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BIAE_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f54 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_E */
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
        /* (0x00002f54 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_E */
        otu_frm_rx_reg_OTU_SK_A_BIAE_E_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_OFF + subfield_offset),
                                                  OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BIAE_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BIAE_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_E_get", stop_bit, 11 );
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
    /* (0x00002f54 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BIAE_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_MSK, OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BIAE_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_A_BEI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_BEI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_BEI_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_BEI_E_set", value );

    /* (0x00002f58 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_E */
    otu_frm_rx_reg_OTU_SK_A_BEI_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_MSK,
                                             OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BEI_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BEI_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f58 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BEI_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_BEI_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BEI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BEI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BEI_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BEI_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BEI_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f58 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_E */
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
        /* (0x00002f58 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_E */
        otu_frm_rx_reg_OTU_SK_A_BEI_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_OFF + subfield_offset),
                                                 OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BEI_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BEI_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BEI_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BEI_E_get", stop_bit, 11 );
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
    /* (0x00002f58 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BEI_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_MSK, OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BEI_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_A_BDI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_BDI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_BDI_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_BDI_E_set", value );

    /* (0x00002f5c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_E */
    otu_frm_rx_reg_OTU_SK_A_BDI_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_MSK,
                                             OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BDI_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BDI_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f5c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BDI_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_BDI_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BDI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BDI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BDI_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BDI_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BDI_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f5c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_E */
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
        /* (0x00002f5c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_E */
        otu_frm_rx_reg_OTU_SK_A_BDI_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_OFF + subfield_offset),
                                                 OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BDI_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BDI_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BDI_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BDI_E_get", stop_bit, 11 );
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
    /* (0x00002f5c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_E */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BDI_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_MSK, OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BDI_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_D_IAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_D_IAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_D_IAE_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_D_IAE_E_set", value );

    /* (0x00002f60 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_E */
    otu_frm_rx_reg_OTU_SK_D_IAE_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_MSK,
                                             OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_IAE_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_IAE_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f60 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_E */
    reg_value = otu_frm_rx_reg_OTU_SK_D_IAE_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_IAE_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_D_IAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_D_IAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_IAE_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_IAE_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_IAE_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f60 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_E */
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
        /* (0x00002f60 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_E */
        otu_frm_rx_reg_OTU_SK_D_IAE_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_OFF + subfield_offset),
                                                 OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_IAE_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_IAE_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_IAE_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_IAE_E_get", stop_bit, 11 );
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
    /* (0x00002f60 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_E */
    reg_value = otu_frm_rx_reg_OTU_SK_D_IAE_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_MSK, OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_IAE_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_D_TIM_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_D_TIM_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_D_TIM_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_D_TIM_E_set", value );

    /* (0x00002f64 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_E */
    otu_frm_rx_reg_OTU_SK_D_TIM_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_MSK,
                                             OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_TIM_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_TIM_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f64 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_E */
    reg_value = otu_frm_rx_reg_OTU_SK_D_TIM_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_TIM_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_D_TIM_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_D_TIM_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_TIM_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_TIM_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_TIM_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f64 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_E */
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
        /* (0x00002f64 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_E */
        otu_frm_rx_reg_OTU_SK_D_TIM_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_OFF + subfield_offset),
                                                 OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_TIM_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_TIM_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_TIM_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_TIM_E_get", stop_bit, 11 );
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
    /* (0x00002f64 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_E */
    reg_value = otu_frm_rx_reg_OTU_SK_D_TIM_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_MSK, OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_TIM_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_D_DEG_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_D_DEG_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_D_DEG_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_D_DEG_E_set", value );

    /* (0x00002f68 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_E */
    otu_frm_rx_reg_OTU_SK_D_DEG_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_MSK,
                                             OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_DEG_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_DEG_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f68 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_E */
    reg_value = otu_frm_rx_reg_OTU_SK_D_DEG_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_DEG_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_D_DEG_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_D_DEG_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_DEG_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_DEG_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_DEG_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f68 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_E */
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
        /* (0x00002f68 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_E */
        otu_frm_rx_reg_OTU_SK_D_DEG_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_OFF + subfield_offset),
                                                 OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_DEG_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_DEG_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_DEG_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_DEG_E_get", stop_bit, 11 );
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
    /* (0x00002f68 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_E */
    reg_value = otu_frm_rx_reg_OTU_SK_D_DEG_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_MSK, OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_DEG_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_D_BDI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_D_BDI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_D_BDI_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_D_BDI_E_set", value );

    /* (0x00002f6c bits 11:0) bits 0:11 use field OTU_SK_D_BDI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_E */
    otu_frm_rx_reg_OTU_SK_D_BDI_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_MSK,
                                             OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BDI_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BDI_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f6c bits 11:0) bits 0:11 use field OTU_SK_D_BDI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_E */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BDI_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_BDI_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_D_BDI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_D_BDI_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BDI_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BDI_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BDI_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f6c bits 11:0) bits 0:11 use field OTU_SK_D_BDI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_E */
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
        /* (0x00002f6c bits 11:0) bits 0:11 use field OTU_SK_D_BDI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_E */
        otu_frm_rx_reg_OTU_SK_D_BDI_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_OFF + subfield_offset),
                                                 OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BDI_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BDI_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BDI_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BDI_E_get", stop_bit, 11 );
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
    /* (0x00002f6c bits 11:0) bits 0:11 use field OTU_SK_D_BDI_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_E */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BDI_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_MSK, OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BDI_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_D_BIAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_D_BIAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_D_BIAE_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_D_BIAE_E_set", value );

    /* (0x00002f70 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_E */
    otu_frm_rx_reg_OTU_SK_D_BIAE_E_field_set( b_ptr,
                                              h_ptr,
                                              OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_MSK,
                                              OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_OFF,
                                              value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BIAE_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BIAE_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f70 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_E */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BIAE_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_BIAE_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_D_BIAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_D_BIAE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BIAE_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f70 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_E */
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
        /* (0x00002f70 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_E */
        otu_frm_rx_reg_OTU_SK_D_BIAE_E_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_OFF + subfield_offset),
                                                  OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BIAE_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BIAE_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_E_get", stop_bit, 11 );
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
    /* (0x00002f70 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_E */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BIAE_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_MSK, OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BIAE_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_CI_SSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_CI_SSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_CI_SSF_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_CI_SSF_E_set", value );

    /* (0x00002f74 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_E */
    otu_frm_rx_reg_OTU_SK_CI_SSF_E_field_set( b_ptr,
                                              h_ptr,
                                              OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_MSK,
                                              OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_OFF,
                                              value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_CI_SSF_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_CI_SSF_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f74 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_E */
    reg_value = otu_frm_rx_reg_OTU_SK_CI_SSF_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_CI_SSF_E_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_CI_SSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_CI_SSF_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_CI_SSF_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f74 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_E */
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
        /* (0x00002f74 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_E */
        otu_frm_rx_reg_OTU_SK_CI_SSF_E_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_OFF + subfield_offset),
                                                  OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_CI_SSF_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_CI_SSF_E_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_E_get", stop_bit, 11 );
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
    /* (0x00002f74 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_E of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_E */
    reg_value = otu_frm_rx_reg_OTU_SK_CI_SSF_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_MSK, OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_CI_SSF_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_rx_field_OTU_SK_COUNT_DONE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_COUNT_DONE_E_set( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_COUNT_DONE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_COUNT_DONE_E_set", value );

    /* (0x00002f78 bits 0) field OTU_SK_COUNT_DONE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E */
    otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_field_set( b_ptr,
                                                   h_ptr,
                                                   OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E_BIT_OTU_SK_COUNT_DONE_E_MSK,
                                                   OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E_BIT_OTU_SK_COUNT_DONE_E_OFF,
                                                   value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_COUNT_DONE_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_COUNT_DONE_E_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f78 bits 0) field OTU_SK_COUNT_DONE_E of register PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E */
    reg_value = otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E_BIT_OTU_SK_COUNT_DONE_E_MSK) >> OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E_BIT_OTU_SK_COUNT_DONE_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_COUNT_DONE_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void otu_frm_rx_field_OTU_SK_A_AIS_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_AIS_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_AIS_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_AIS_I_set_to_clear", value );

    /* (0x00002f80 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I */
    otu_frm_rx_reg_OTU_SK_A_AIS_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_MSK,
                                                             OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_AIS_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_AIS_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f80 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_AIS_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_AIS_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_AIS_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_AIS_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_AIS_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_AIS_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_AIS_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f80 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I */
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
        /* (0x00002f80 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I */
        otu_frm_rx_reg_OTU_SK_A_AIS_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_OFF + subfield_offset),
                                                                 OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_AIS_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_AIS_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_AIS_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_AIS_I_get", stop_bit, 11 );
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
    /* (0x00002f80 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_AIS_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_MSK, OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_AIS_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_AIS_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_AIS_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_AIS_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_AIS_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_AIS_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f80 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002f80 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I */
        return otu_frm_rx_reg_OTU_SK_A_AIS_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_AIS_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_AIS_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_AIS_I_poll", value );

    /* (0x00002f80 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I */
    return otu_frm_rx_reg_OTU_SK_A_AIS_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_A_SSD_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_SSD_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSD_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSD_I_set_to_clear", value );

    /* (0x00002f84 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I */
    otu_frm_rx_reg_OTU_SK_A_SSD_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_MSK,
                                                             OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSD_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSD_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f84 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSD_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSD_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_SSD_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_SSD_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSD_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSD_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSD_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f84 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I */
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
        /* (0x00002f84 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I */
        otu_frm_rx_reg_OTU_SK_A_SSD_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_OFF + subfield_offset),
                                                                 OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSD_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSD_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSD_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSD_I_get", stop_bit, 11 );
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
    /* (0x00002f84 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSD_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_MSK, OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSD_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSD_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSD_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSD_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f84 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002f84 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I */
        return otu_frm_rx_reg_OTU_SK_A_SSD_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_SSD_I_poll", value );

    /* (0x00002f84 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I */
    return otu_frm_rx_reg_OTU_SK_A_SSD_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_A_SSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_SSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSF_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSF_I_set_to_clear", value );

    /* (0x00002f88 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I */
    otu_frm_rx_reg_OTU_SK_A_SSF_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_MSK,
                                                             OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSF_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSF_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f88 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSF_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSF_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_SSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_SSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSF_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSF_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSF_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f88 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I */
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
        /* (0x00002f88 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I */
        otu_frm_rx_reg_OTU_SK_A_SSF_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_OFF + subfield_offset),
                                                                 OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSF_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSF_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSF_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSF_I_get", stop_bit, 11 );
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
    /* (0x00002f88 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSF_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_MSK, OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSF_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSF_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSF_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSF_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f88 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002f88 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I */
        return otu_frm_rx_reg_OTU_SK_A_SSF_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_SSF_I_poll", value );

    /* (0x00002f88 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I */
    return otu_frm_rx_reg_OTU_SK_A_SSF_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_A_TSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_TSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSF_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSF_I_set_to_clear", value );

    /* (0x00002f8c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I */
    otu_frm_rx_reg_OTU_SK_A_TSF_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_MSK,
                                                             OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSF_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSF_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f8c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSF_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSF_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_TSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_TSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSF_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSF_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSF_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f8c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I */
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
        /* (0x00002f8c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I */
        otu_frm_rx_reg_OTU_SK_A_TSF_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_OFF + subfield_offset),
                                                                 OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSF_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSF_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSF_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSF_I_get", stop_bit, 11 );
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
    /* (0x00002f8c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSF_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_MSK, OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSF_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSF_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSF_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSF_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f8c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002f8c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I */
        return otu_frm_rx_reg_OTU_SK_A_TSF_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_TSF_I_poll", value );

    /* (0x00002f8c bits 11:0) bits 0:11 use field OTU_SK_A_TSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I */
    return otu_frm_rx_reg_OTU_SK_A_TSF_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_A_TSD_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_TSD_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSD_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSD_I_set_to_clear", value );

    /* (0x00002f90 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I */
    otu_frm_rx_reg_OTU_SK_A_TSD_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_MSK,
                                                             OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSD_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSD_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f90 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSD_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSD_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_TSD_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_TSD_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSD_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSD_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSD_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f90 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I */
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
        /* (0x00002f90 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I */
        otu_frm_rx_reg_OTU_SK_A_TSD_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_OFF + subfield_offset),
                                                                 OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSD_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSD_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSD_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSD_I_get", stop_bit, 11 );
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
    /* (0x00002f90 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSD_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_MSK, OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSD_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSD_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSD_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSD_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f90 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002f90 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I */
        return otu_frm_rx_reg_OTU_SK_A_TSD_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSD_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_TSD_I_poll", value );

    /* (0x00002f90 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I */
    return otu_frm_rx_reg_OTU_SK_A_TSD_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_A_BIAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_BIAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_BIAE_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_BIAE_I_set_to_clear", value );

    /* (0x00002f94 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I */
    otu_frm_rx_reg_OTU_SK_A_BIAE_I_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_MSK,
                                                              OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_OFF,
                                                              value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BIAE_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BIAE_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f94 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BIAE_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_BIAE_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BIAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BIAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BIAE_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f94 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I */
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
        /* (0x00002f94 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I */
        otu_frm_rx_reg_OTU_SK_A_BIAE_I_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_OFF + subfield_offset),
                                                                  OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BIAE_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BIAE_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_I_get", stop_bit, 11 );
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
    /* (0x00002f94 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BIAE_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_MSK, OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BIAE_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BIAE_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f94 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002f94 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I */
        return otu_frm_rx_reg_OTU_SK_A_BIAE_I_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_OFF + subfield_offset),
                                                    value << (OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_BIAE_I_poll", value );

    /* (0x00002f94 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I */
    return otu_frm_rx_reg_OTU_SK_A_BIAE_I_poll( b_ptr,
                                                h_ptr,
                                                OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_MSK,
                                                (value<<OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_A_BEI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_BEI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_BEI_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_BEI_I_set_to_clear", value );

    /* (0x00002f98 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I */
    otu_frm_rx_reg_OTU_SK_A_BEI_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_MSK,
                                                             OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BEI_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BEI_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f98 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BEI_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_BEI_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BEI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BEI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BEI_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BEI_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BEI_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f98 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I */
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
        /* (0x00002f98 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I */
        otu_frm_rx_reg_OTU_SK_A_BEI_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_OFF + subfield_offset),
                                                                 OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BEI_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BEI_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BEI_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BEI_I_get", stop_bit, 11 );
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
    /* (0x00002f98 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BEI_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_MSK, OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BEI_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BEI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BEI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BEI_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BEI_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BEI_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f98 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002f98 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I */
        return otu_frm_rx_reg_OTU_SK_A_BEI_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BEI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BEI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_BEI_I_poll", value );

    /* (0x00002f98 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I */
    return otu_frm_rx_reg_OTU_SK_A_BEI_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_A_BDI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_A_BDI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_A_BDI_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_A_BDI_I_set_to_clear", value );

    /* (0x00002f9c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I */
    otu_frm_rx_reg_OTU_SK_A_BDI_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_MSK,
                                                             OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BDI_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BDI_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002f9c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BDI_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_BDI_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BDI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_A_BDI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BDI_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BDI_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BDI_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f9c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I */
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
        /* (0x00002f9c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I */
        otu_frm_rx_reg_OTU_SK_A_BDI_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_OFF + subfield_offset),
                                                                 OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BDI_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BDI_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BDI_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BDI_I_get", stop_bit, 11 );
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
    /* (0x00002f9c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BDI_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_MSK, OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BDI_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BDI_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BDI_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BDI_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002f9c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002f9c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I */
        return otu_frm_rx_reg_OTU_SK_A_BDI_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_BDI_I_poll", value );

    /* (0x00002f9c bits 11:0) bits 0:11 use field OTU_SK_A_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I */
    return otu_frm_rx_reg_OTU_SK_A_BDI_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_D_IAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_D_IAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_D_IAE_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_D_IAE_I_set_to_clear", value );

    /* (0x00002fa0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I */
    otu_frm_rx_reg_OTU_SK_D_IAE_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_MSK,
                                                             OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_IAE_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_IAE_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fa0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I */
    reg_value = otu_frm_rx_reg_OTU_SK_D_IAE_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_IAE_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_D_IAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_D_IAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_IAE_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_IAE_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_IAE_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fa0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I */
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
        /* (0x00002fa0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I */
        otu_frm_rx_reg_OTU_SK_D_IAE_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_OFF + subfield_offset),
                                                                 OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_IAE_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_IAE_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_IAE_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_IAE_I_get", stop_bit, 11 );
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
    /* (0x00002fa0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I */
    reg_value = otu_frm_rx_reg_OTU_SK_D_IAE_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_MSK, OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_IAE_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_IAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_IAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_IAE_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_IAE_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_IAE_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fa0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fa0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I */
        return otu_frm_rx_reg_OTU_SK_D_IAE_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_IAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_IAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_D_IAE_I_poll", value );

    /* (0x00002fa0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I */
    return otu_frm_rx_reg_OTU_SK_D_IAE_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_D_TIM_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_D_TIM_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_D_TIM_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_D_TIM_I_set_to_clear", value );

    /* (0x00002fa4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I */
    otu_frm_rx_reg_OTU_SK_D_TIM_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_MSK,
                                                             OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_TIM_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_TIM_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fa4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I */
    reg_value = otu_frm_rx_reg_OTU_SK_D_TIM_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_TIM_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_D_TIM_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_D_TIM_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_TIM_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_TIM_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_TIM_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fa4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I */
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
        /* (0x00002fa4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I */
        otu_frm_rx_reg_OTU_SK_D_TIM_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_OFF + subfield_offset),
                                                                 OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_TIM_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_TIM_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_TIM_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_TIM_I_get", stop_bit, 11 );
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
    /* (0x00002fa4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I */
    reg_value = otu_frm_rx_reg_OTU_SK_D_TIM_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_MSK, OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_TIM_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_TIM_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_TIM_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_TIM_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_TIM_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_TIM_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fa4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fa4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I */
        return otu_frm_rx_reg_OTU_SK_D_TIM_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_TIM_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_TIM_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_D_TIM_I_poll", value );

    /* (0x00002fa4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I */
    return otu_frm_rx_reg_OTU_SK_D_TIM_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_D_DEG_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_D_DEG_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_D_DEG_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_D_DEG_I_set_to_clear", value );

    /* (0x00002fa8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I */
    otu_frm_rx_reg_OTU_SK_D_DEG_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_MSK,
                                                             OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_DEG_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_DEG_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fa8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I */
    reg_value = otu_frm_rx_reg_OTU_SK_D_DEG_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_DEG_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_D_DEG_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_D_DEG_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_DEG_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_DEG_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_DEG_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fa8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I */
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
        /* (0x00002fa8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I */
        otu_frm_rx_reg_OTU_SK_D_DEG_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_OFF + subfield_offset),
                                                                 OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_DEG_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_DEG_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_DEG_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_DEG_I_get", stop_bit, 11 );
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
    /* (0x00002fa8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I */
    reg_value = otu_frm_rx_reg_OTU_SK_D_DEG_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_MSK, OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_DEG_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_DEG_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_DEG_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_DEG_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_DEG_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_DEG_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fa8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fa8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I */
        return otu_frm_rx_reg_OTU_SK_D_DEG_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_DEG_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_DEG_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_D_DEG_I_poll", value );

    /* (0x00002fa8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I */
    return otu_frm_rx_reg_OTU_SK_D_DEG_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_D_BDI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_D_BDI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_D_BDI_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_D_BDI_I_set_to_clear", value );

    /* (0x00002fac bits 11:0) bits 0:11 use field OTU_SK_D_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I */
    otu_frm_rx_reg_OTU_SK_D_BDI_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_MSK,
                                                             OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BDI_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BDI_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fac bits 11:0) bits 0:11 use field OTU_SK_D_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BDI_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_BDI_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_D_BDI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_D_BDI_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BDI_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BDI_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BDI_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fac bits 11:0) bits 0:11 use field OTU_SK_D_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I */
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
        /* (0x00002fac bits 11:0) bits 0:11 use field OTU_SK_D_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I */
        otu_frm_rx_reg_OTU_SK_D_BDI_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_OFF + subfield_offset),
                                                                 OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BDI_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BDI_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BDI_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BDI_I_get", stop_bit, 11 );
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
    /* (0x00002fac bits 11:0) bits 0:11 use field OTU_SK_D_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BDI_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_MSK, OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BDI_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BDI_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BDI_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BDI_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fac bits 11:0) bits 0:11 use field OTU_SK_D_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fac bits 11:0) bits 0:11 use field OTU_SK_D_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I */
        return otu_frm_rx_reg_OTU_SK_D_BDI_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BDI_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_D_BDI_I_poll", value );

    /* (0x00002fac bits 11:0) bits 0:11 use field OTU_SK_D_BDI_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I */
    return otu_frm_rx_reg_OTU_SK_D_BDI_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_D_BIAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_D_BIAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_D_BIAE_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_D_BIAE_I_set_to_clear", value );

    /* (0x00002fb0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I */
    otu_frm_rx_reg_OTU_SK_D_BIAE_I_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_MSK,
                                                              OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_OFF,
                                                              value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BIAE_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BIAE_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fb0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BIAE_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_BIAE_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_D_BIAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_D_BIAE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BIAE_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fb0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I */
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
        /* (0x00002fb0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I */
        otu_frm_rx_reg_OTU_SK_D_BIAE_I_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_OFF + subfield_offset),
                                                                  OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BIAE_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BIAE_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_I_get", stop_bit, 11 );
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
    /* (0x00002fb0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BIAE_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_MSK, OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BIAE_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BIAE_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fb0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fb0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I */
        return otu_frm_rx_reg_OTU_SK_D_BIAE_I_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_OFF + subfield_offset),
                                                    value << (OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BIAE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_D_BIAE_I_poll", value );

    /* (0x00002fb0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I */
    return otu_frm_rx_reg_OTU_SK_D_BIAE_I_poll( b_ptr,
                                                h_ptr,
                                                OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_MSK,
                                                (value<<OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_CI_SSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_CI_SSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_CI_SSF_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_CI_SSF_I_set_to_clear", value );

    /* (0x00002fb4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I */
    otu_frm_rx_reg_OTU_SK_CI_SSF_I_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_MSK,
                                                              OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_OFF,
                                                              value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_CI_SSF_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_CI_SSF_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fb4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I */
    reg_value = otu_frm_rx_reg_OTU_SK_CI_SSF_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_CI_SSF_I_get", value );

    return value;
}
static INLINE void otu_frm_rx_field_range_OTU_SK_CI_SSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_range_OTU_SK_CI_SSF_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_CI_SSF_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fb4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I */
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
        /* (0x00002fb4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I */
        otu_frm_rx_reg_OTU_SK_CI_SSF_I_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_OFF + subfield_offset),
                                                                  OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_CI_SSF_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_CI_SSF_I_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_I_get", stop_bit, 11 );
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
    /* (0x00002fb4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I */
    reg_value = otu_frm_rx_reg_OTU_SK_CI_SSF_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_MSK, OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_CI_SSF_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_CI_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_CI_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_CI_SSF_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fb4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fb4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I */
        return otu_frm_rx_reg_OTU_SK_CI_SSF_I_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_OFF + subfield_offset),
                                                    value << (OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_CI_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_CI_SSF_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_CI_SSF_I_poll", value );

    /* (0x00002fb4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_I of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I */
    return otu_frm_rx_reg_OTU_SK_CI_SSF_I_poll( b_ptr,
                                                h_ptr,
                                                OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_MSK,
                                                (value<<OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_rx_field_OTU_SK_COUNT_DONE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_field_OTU_SK_COUNT_DONE_I_set_to_clear( otu_frm_rx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_rx_field_OTU_SK_COUNT_DONE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_rx_field_OTU_SK_COUNT_DONE_I_set_to_clear", value );

    /* (0x00002fb8 bits 0) field OTU_SK_COUNT_DONE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I */
    otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I_BIT_OTU_SK_COUNT_DONE_I_MSK,
                                                                   OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I_BIT_OTU_SK_COUNT_DONE_I_OFF,
                                                                   value);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_COUNT_DONE_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_COUNT_DONE_I_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fb8 bits 0) field OTU_SK_COUNT_DONE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I */
    reg_value = otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I_BIT_OTU_SK_COUNT_DONE_I_MSK) >> OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I_BIT_OTU_SK_COUNT_DONE_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_COUNT_DONE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_COUNT_DONE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                              otu_frm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_COUNT_DONE_I_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                              otu_frm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_COUNT_DONE_I_poll", value );

    /* (0x00002fb8 bits 0) field OTU_SK_COUNT_DONE_I of register PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I */
    return otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_poll( b_ptr,
                                                     h_ptr,
                                                     OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I_BIT_OTU_SK_COUNT_DONE_I_MSK,
                                                     (value<<OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I_BIT_OTU_SK_COUNT_DONE_I_OFF),
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
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_AIS_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_AIS_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fc0 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_AIS_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_AIS_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_AIS_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_AIS_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_AIS_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_AIS_V_get", stop_bit, 11 );
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
    /* (0x00002fc0 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_AIS_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_MSK, OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_AIS_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_AIS_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_AIS_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_AIS_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_AIS_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_AIS_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fc0 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fc0 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_V */
        return otu_frm_rx_reg_OTU_SK_A_AIS_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_AIS_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_AIS_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_AIS_V_poll", value );

    /* (0x00002fc0 bits 11:0) bits 0:11 use field OTU_SK_A_AIS_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_V */
    return otu_frm_rx_reg_OTU_SK_A_AIS_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSD_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSD_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fc4 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSD_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSD_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSD_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSD_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSD_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSD_V_get", stop_bit, 11 );
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
    /* (0x00002fc4 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSD_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_MSK, OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSD_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSD_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSD_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSD_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fc4 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fc4 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_V */
        return otu_frm_rx_reg_OTU_SK_A_SSD_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_SSD_V_poll", value );

    /* (0x00002fc4 bits 11:0) bits 0:11 use field OTU_SK_A_SSD_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_V */
    return otu_frm_rx_reg_OTU_SK_A_SSD_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSF_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_SSF_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fc8 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSF_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_SSF_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSF_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_SSF_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSF_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSF_V_get", stop_bit, 11 );
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
    /* (0x00002fc8 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_SSF_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_MSK, OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSF_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_SSF_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_SSF_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_SSF_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fc8 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fc8 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_V */
        return otu_frm_rx_reg_OTU_SK_A_SSF_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_SSF_V_poll", value );

    /* (0x00002fc8 bits 11:0) bits 0:11 use field OTU_SK_A_SSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_V */
    return otu_frm_rx_reg_OTU_SK_A_SSF_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSF_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSF_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fcc bits 11:0) bits 0:11 use field OTU_SK_A_TSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSF_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSF_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSF_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSF_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSF_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSF_V_get", stop_bit, 11 );
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
    /* (0x00002fcc bits 11:0) bits 0:11 use field OTU_SK_A_TSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSF_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_MSK, OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSF_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSF_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSF_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSF_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fcc bits 11:0) bits 0:11 use field OTU_SK_A_TSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fcc bits 11:0) bits 0:11 use field OTU_SK_A_TSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_V */
        return otu_frm_rx_reg_OTU_SK_A_TSF_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_TSF_V_poll", value );

    /* (0x00002fcc bits 11:0) bits 0:11 use field OTU_SK_A_TSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_V */
    return otu_frm_rx_reg_OTU_SK_A_TSF_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSD_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_TSD_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fd0 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSD_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_TSD_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSD_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_TSD_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSD_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSD_V_get", stop_bit, 11 );
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
    /* (0x00002fd0 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_TSD_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_MSK, OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSD_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_TSD_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_TSD_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_TSD_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fd0 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fd0 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_V */
        return otu_frm_rx_reg_OTU_SK_A_TSD_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSD_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_TSD_V_poll", value );

    /* (0x00002fd0 bits 11:0) bits 0:11 use field OTU_SK_A_TSD_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_V */
    return otu_frm_rx_reg_OTU_SK_A_TSD_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BIAE_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BIAE_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fd4 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BIAE_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_BIAE_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BIAE_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BIAE_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_V_get", stop_bit, 11 );
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
    /* (0x00002fd4 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BIAE_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_MSK, OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BIAE_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BIAE_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BIAE_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fd4 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fd4 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_V */
        return otu_frm_rx_reg_OTU_SK_A_BIAE_V_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_OFF + subfield_offset),
                                                    value << (OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_BIAE_V_poll", value );

    /* (0x00002fd4 bits 11:0) bits 0:11 use field OTU_SK_A_BIAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_V */
    return otu_frm_rx_reg_OTU_SK_A_BIAE_V_poll( b_ptr,
                                                h_ptr,
                                                OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_MSK,
                                                (value<<OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BEI_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BEI_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fd8 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BEI_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_BEI_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BEI_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BEI_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BEI_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BEI_V_get", stop_bit, 11 );
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
    /* (0x00002fd8 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BEI_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_MSK, OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BEI_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BEI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BEI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BEI_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BEI_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BEI_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fd8 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fd8 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_V */
        return otu_frm_rx_reg_OTU_SK_A_BEI_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BEI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BEI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_BEI_V_poll", value );

    /* (0x00002fd8 bits 11:0) bits 0:11 use field OTU_SK_A_BEI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_V */
    return otu_frm_rx_reg_OTU_SK_A_BEI_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BDI_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_A_BDI_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fdc bits 11:0) bits 0:11 use field OTU_SK_A_BDI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BDI_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_A_BDI_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BDI_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_A_BDI_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BDI_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BDI_V_get", stop_bit, 11 );
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
    /* (0x00002fdc bits 11:0) bits 0:11 use field OTU_SK_A_BDI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_V */
    reg_value = otu_frm_rx_reg_OTU_SK_A_BDI_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_MSK, OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BDI_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_A_BDI_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_A_BDI_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_A_BDI_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fdc bits 11:0) bits 0:11 use field OTU_SK_A_BDI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fdc bits 11:0) bits 0:11 use field OTU_SK_A_BDI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_V */
        return otu_frm_rx_reg_OTU_SK_A_BDI_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_A_BDI_V_poll", value );

    /* (0x00002fdc bits 11:0) bits 0:11 use field OTU_SK_A_BDI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_V */
    return otu_frm_rx_reg_OTU_SK_A_BDI_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_IAE_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_IAE_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fe0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_V */
    reg_value = otu_frm_rx_reg_OTU_SK_D_IAE_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_IAE_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_IAE_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_IAE_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_IAE_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_IAE_V_get", stop_bit, 11 );
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
    /* (0x00002fe0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_V */
    reg_value = otu_frm_rx_reg_OTU_SK_D_IAE_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_MSK, OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_IAE_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_IAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_IAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_IAE_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_IAE_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_IAE_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fe0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fe0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_V */
        return otu_frm_rx_reg_OTU_SK_D_IAE_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_IAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_IAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_D_IAE_V_poll", value );

    /* (0x00002fe0 bits 11:0) bits 0:11 use field OTU_SK_D_IAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_V */
    return otu_frm_rx_reg_OTU_SK_D_IAE_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_TIM_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_TIM_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fe4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_V */
    reg_value = otu_frm_rx_reg_OTU_SK_D_TIM_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_TIM_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_TIM_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_TIM_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_TIM_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_TIM_V_get", stop_bit, 11 );
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
    /* (0x00002fe4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_V */
    reg_value = otu_frm_rx_reg_OTU_SK_D_TIM_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_MSK, OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_TIM_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_TIM_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_TIM_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_TIM_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_TIM_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_TIM_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fe4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fe4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_V */
        return otu_frm_rx_reg_OTU_SK_D_TIM_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_TIM_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_TIM_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_D_TIM_V_poll", value );

    /* (0x00002fe4 bits 11:0) bits 0:11 use field OTU_SK_D_TIM_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_V */
    return otu_frm_rx_reg_OTU_SK_D_TIM_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_DEG_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_DEG_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fe8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_V */
    reg_value = otu_frm_rx_reg_OTU_SK_D_DEG_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_DEG_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_DEG_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_DEG_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_DEG_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_DEG_V_get", stop_bit, 11 );
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
    /* (0x00002fe8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_V */
    reg_value = otu_frm_rx_reg_OTU_SK_D_DEG_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_MSK, OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_DEG_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_DEG_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_DEG_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_DEG_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_DEG_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_DEG_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fe8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fe8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_V */
        return otu_frm_rx_reg_OTU_SK_D_DEG_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_DEG_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_DEG_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_D_DEG_V_poll", value );

    /* (0x00002fe8 bits 11:0) bits 0:11 use field OTU_SK_D_DEG_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_V */
    return otu_frm_rx_reg_OTU_SK_D_DEG_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BDI_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BDI_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002fec bits 11:0) bits 0:11 use field OTU_SK_D_BDI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_V */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BDI_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_BDI_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BDI_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BDI_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BDI_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BDI_V_get", stop_bit, 11 );
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
    /* (0x00002fec bits 11:0) bits 0:11 use field OTU_SK_D_BDI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_V */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BDI_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_MSK, OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BDI_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BDI_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BDI_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BDI_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002fec bits 11:0) bits 0:11 use field OTU_SK_D_BDI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002fec bits 11:0) bits 0:11 use field OTU_SK_D_BDI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_V */
        return otu_frm_rx_reg_OTU_SK_D_BDI_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BDI_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_D_BDI_V_poll", value );

    /* (0x00002fec bits 11:0) bits 0:11 use field OTU_SK_D_BDI_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_V */
    return otu_frm_rx_reg_OTU_SK_D_BDI_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_MSK,
                                               (value<<OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BIAE_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_D_BIAE_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002ff0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_V */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BIAE_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_D_BIAE_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BIAE_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_D_BIAE_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_V_get", stop_bit, 11 );
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
    /* (0x00002ff0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_V */
    reg_value = otu_frm_rx_reg_OTU_SK_D_BIAE_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_MSK, OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BIAE_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_D_BIAE_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_D_BIAE_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002ff0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002ff0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_V */
        return otu_frm_rx_reg_OTU_SK_D_BIAE_V_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_OFF + subfield_offset),
                                                    value << (OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BIAE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_D_BIAE_V_poll", value );

    /* (0x00002ff0 bits 11:0) bits 0:11 use field OTU_SK_D_BIAE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_V */
    return otu_frm_rx_reg_OTU_SK_D_BIAE_V_poll( b_ptr,
                                                h_ptr,
                                                OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_MSK,
                                                (value<<OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_CI_SSF_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_CI_SSF_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002ff4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_V */
    reg_value = otu_frm_rx_reg_OTU_SK_CI_SSF_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_CI_SSF_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_CI_SSF_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_CI_SSF_V_get( otu_frm_rx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_V_get", stop_bit, 11 );
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
    /* (0x00002ff4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_V */
    reg_value = otu_frm_rx_reg_OTU_SK_CI_SSF_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_MSK, OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_CI_SSF_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_CI_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_CI_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_CI_SSF_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_CI_SSF_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002ff4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_V */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002ff4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_V */
        return otu_frm_rx_reg_OTU_SK_CI_SSF_V_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_OFF + subfield_offset),
                                                    value << (OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_CI_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_CI_SSF_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_CI_SSF_V_poll", value );

    /* (0x00002ff4 bits 11:0) bits 0:11 use field OTU_SK_CI_SSF_V of register PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_V */
    return otu_frm_rx_reg_OTU_SK_CI_SSF_V_poll( b_ptr,
                                                h_ptr,
                                                OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_MSK,
                                                (value<<OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_rx_field_OTU_SK_COUNT_DONE_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_COUNT_DONE_V_get( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002ff8 bits 0) field OTU_SK_COUNT_DONE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V */
    reg_value = otu_frm_rx_reg_OTU_SK_SHADOW_XFER_V_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V_BIT_OTU_SK_COUNT_DONE_V_MSK) >> OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V_BIT_OTU_SK_COUNT_DONE_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_rx_field_OTU_SK_COUNT_DONE_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_COUNT_DONE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                              otu_frm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_COUNT_DONE_V_poll( otu_frm_rx_buffer_t *b_ptr,
                                                                              otu_frm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_rx_field_OTU_SK_COUNT_DONE_V_poll", value );

    /* (0x00002ff8 bits 0) field OTU_SK_COUNT_DONE_V of register PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V */
    return otu_frm_rx_reg_OTU_SK_SHADOW_XFER_V_poll( b_ptr,
                                                     h_ptr,
                                                     OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V_BIT_OTU_SK_COUNT_DONE_V_MSK,
                                                     (value<<OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V_BIT_OTU_SK_COUNT_DONE_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size12
 * ==================================================================================
 */
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_TTI_get", N, 11);
    /* ((0x000020c8 + (N) * 0x100) bits 23:16) field OTU_SK_SM_TTI of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_OH_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_TTI_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_TTI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_TTI_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_SM_TTI_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_SM_TTI_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_SM_TTI_get", stop_bit, 7 );
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
    /* ((0x000020c8 + (N) * 0x100) bits 23:16) field OTU_SK_SM_TTI of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_OH_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_TTI_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_TTI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_TTI_MSK, OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_TTI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_SM_TTI_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_BIP8_get( otu_frm_rx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_BIP8_get( otu_frm_rx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_BIP8_get", N, 11);
    /* ((0x000020c8 + (N) * 0x100) bits 15:8) field OTU_SK_SM_BIP8 of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_OH_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BIP8_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BIP8_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_BIP8_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_BIP8_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_BIP8_get( otu_frm_rx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_SM_BIP8_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_SM_BIP8_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_SM_BIP8_get", stop_bit, 7 );
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
    /* ((0x000020c8 + (N) * 0x100) bits 15:8) field OTU_SK_SM_BIP8 of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_OH_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BIP8_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BIP8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BIP8_MSK, OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BIP8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_SM_BIP8_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_BEI_BIAE_get( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_BEI_BIAE_get( otu_frm_rx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_BEI_BIAE_get", N, 11);
    /* ((0x000020c8 + (N) * 0x100) bits 7:4) field OTU_SK_SM_BEI_BIAE of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_OH_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BEI_BIAE_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BEI_BIAE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_BEI_BIAE_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_BEI_BIAE_get( otu_frm_rx_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_BEI_BIAE_get( otu_frm_rx_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_SM_BEI_BIAE_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_SM_BEI_BIAE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_SM_BEI_BIAE_get", stop_bit, 3 );
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
    /* ((0x000020c8 + (N) * 0x100) bits 7:4) field OTU_SK_SM_BEI_BIAE of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_OH_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BEI_BIAE_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BEI_BIAE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BEI_BIAE_MSK, OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BEI_BIAE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_SM_BEI_BIAE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_BDI_get( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_BDI_get( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_BDI_get", N, 11);
    /* ((0x000020c8 + (N) * 0x100) bits 3) field OTU_SK_SM_BDI of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_OH_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BDI_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BDI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_BDI_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_IAE_get( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_IAE_get( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_IAE_get", N, 11);
    /* ((0x000020c8 + (N) * 0x100) bits 2) field OTU_SK_SM_IAE of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_OH_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_IAE_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_IAE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_IAE_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_RES_get( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_RES_get( otu_frm_rx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_RES_get", N, 11);
    /* ((0x000020c8 + (N) * 0x100) bits 1:0) field OTU_SK_SM_RES of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_OH_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_RES_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_RES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_RES_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_RES_get( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_RES_get( otu_frm_rx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_SM_RES_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_SM_RES_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_SM_RES_get", stop_bit, 1 );
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
    /* ((0x000020c8 + (N) * 0x100) bits 1:0) field OTU_SK_SM_RES of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_OH_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_RES_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_RES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_RES_MSK, OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_RES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_SM_RES_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_GCC0_get( otu_frm_rx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_GCC0_get( otu_frm_rx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_GCC0_get", N, 11);
    /* ((0x000020cc + (N) * 0x100) bits 31:16) field OTU_SK_GCC0 of register PMC_OTU_FRM_RX_REG_OTU_SK_OH2 index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_OH2_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_GCC0_MSK) >> OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_GCC0_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_GCC0_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_GCC0_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_GCC0_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_GCC0_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_GCC0_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_GCC0_get", stop_bit, 15 );
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
    /* ((0x000020cc + (N) * 0x100) bits 31:16) field OTU_SK_GCC0 of register PMC_OTU_FRM_RX_REG_OTU_SK_OH2 index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_OH2_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_GCC0_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_GCC0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_GCC0_MSK, OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_GCC0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_GCC0_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_RES_get( otu_frm_rx_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_RES_get( otu_frm_rx_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_RES_get", N, 11);
    /* ((0x000020cc + (N) * 0x100) bits 15:0) field OTU_SK_RES of register PMC_OTU_FRM_RX_REG_OTU_SK_OH2 index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_OH2_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_RES_MSK) >> OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_RES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_RES_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_RES_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_RES_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_RES_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_RES_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_RES_get", stop_bit, 15 );
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
    /* ((0x000020cc + (N) * 0x100) bits 15:0) field OTU_SK_RES of register PMC_OTU_FRM_RX_REG_OTU_SK_OH2 index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_OH2_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_RES_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_RES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_RES_MSK, OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_RES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_RES_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size12__M_burstsize16
 * ==================================================================================
 */
static INLINE void otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value[16] ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_set( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value[16] )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_set", N, 11);
    IOLOG( "%s  N=%d", "otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_set", N);
    otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_write( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        0,
                                                        16,
                                                        value);
}

static INLINE void otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value[16] ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value[16] )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_get", N, 11);
    IOLOG( "%s  N=%d", "otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_get", N);
    otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_read( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       0,
                                                       16,
                                                       (UINT32 *)value);
}

static INLINE void otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_set( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_set( otu_frm_rx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_set", N, 11);
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d value=0x%08x", "otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_set", N, start_bit, stop_bit, value);
    otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      word_number,
                                                      mask,
                                                      field_ofs,
                                                      value);
}

static INLINE UINT32 otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_get", N, 11);
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_get", start_bit, stop_bit );
    }
    reg_value = otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_read( b_ptr,
                                                             h_ptr,
                                                             N,
                                                             word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d value=0x%08x", "otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_get", N, start_bit, stop_bit, value );
    return value;
}

static INLINE void otu_frm_rx_lfield_OTU_SK_SM_AC_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value[16] ) ALWAYS_INLINE;
static INLINE void otu_frm_rx_lfield_OTU_SK_SM_AC_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value[16] )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_lfield_OTU_SK_SM_AC_TTI_get", N, 11);
    IOLOG( "%s  N=%d", "otu_frm_rx_lfield_OTU_SK_SM_AC_TTI_get", N);
    otu_frm_rx_reg_OTU_SK_SM_AC_TTI_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      0,
                                                      16,
                                                      (UINT32 *)value);
}

static INLINE UINT32 otu_frm_rx_lfield_range_OTU_SK_SM_AC_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_lfield_range_OTU_SK_SM_AC_TTI_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_lfield_range_OTU_SK_SM_AC_TTI_get", N, 11);
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "otu_frm_rx_lfield_range_OTU_SK_SM_AC_TTI_get", start_bit, stop_bit );
    }
    reg_value = otu_frm_rx_reg_OTU_SK_SM_AC_TTI_array_read( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d value=0x%08x", "otu_frm_rx_lfield_range_OTU_SK_SM_AC_TTI_get", N, start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size12
 * ==================================================================================
 */
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_IAE_get( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_IAE_get( otu_frm_rx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_P_IAE_get", N, 11);
    /* ((0x000020c0 + (N) * 0x100) bits 25) field OTU_SK_SM_P_IAE of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_PMON_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_IAE_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_IAE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_P_IAE_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_N_DS_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_N_DS_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_P_N_DS_get", N, 11);
    /* ((0x000020c0 + (N) * 0x100) bits 24) field OTU_SK_SM_P_N_DS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_PMON_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_DS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_DS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_P_N_DS_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_N_EBC_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_N_EBC_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_P_N_EBC_get", N, 11);
    /* ((0x000020c0 + (N) * 0x100) bits 23:0) bits 0:23 use field OTU_SK_SM_P_N_EBC of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_PMON_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_EBC_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_EBC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_P_N_EBC_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_P_N_EBC_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_P_N_EBC_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_SM_P_N_EBC_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_SM_P_N_EBC_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_SM_P_N_EBC_get", stop_bit, 23 );
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
    /* ((0x000020c0 + (N) * 0x100) bits 23:0) bits 0:23 use field OTU_SK_SM_P_N_EBC of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_PMON_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_EBC_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_EBC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_EBC_MSK, OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_EBC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_SM_P_N_EBC_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_BIAE_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_BIAE_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_P_BIAE_get", N, 11);
    /* ((0x000020c4 + (N) * 0x100) bits 25) field OTU_SK_SM_P_BIAE of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_FE_PMON_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_BIAE_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_BIAE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_P_BIAE_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_F_DS_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_F_DS_get( otu_frm_rx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_P_F_DS_get", N, 11);
    /* ((0x000020c4 + (N) * 0x100) bits 24) field OTU_SK_SM_P_F_DS of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_FE_PMON_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_DS_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_DS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_P_F_DS_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_F_EBC_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_OTU_SK_SM_P_F_EBC_get( otu_frm_rx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_OTU_SK_SM_P_F_EBC_get", N, 11);
    /* ((0x000020c4 + (N) * 0x100) bits 23:0) bits 0:23 use field OTU_SK_SM_P_F_EBC of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_FE_PMON_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_EBC_MSK) >> OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_EBC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_rx_field_OTU_SK_SM_P_F_EBC_get", N, value );

    return value;
}
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_P_F_EBC_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_rx_field_range_OTU_SK_SM_P_F_EBC_get( otu_frm_rx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_rx_field_range_OTU_SK_SM_P_F_EBC_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_rx_field_range_OTU_SK_SM_P_F_EBC_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_rx_field_range_OTU_SK_SM_P_F_EBC_get", stop_bit, 23 );
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
    /* ((0x000020c4 + (N) * 0x100) bits 23:0) bits 0:23 use field OTU_SK_SM_P_F_EBC of register PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON index N=0..11 */
    reg_value = otu_frm_rx_reg_OTU_SK_SM_FE_PMON_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_EBC_MSK)
                  >> OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_EBC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_EBC_MSK, OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_EBC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_rx_field_range_OTU_SK_SM_P_F_EBC_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OTU_FRM_RX_IO_INLINE_H */
