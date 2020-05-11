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
 *     and register accessor functions for the otu_frm_tx block
 *****************************************************************************/
#ifndef _OTU_FRM_TX_IO_INLINE_H
#define _OTU_FRM_TX_IO_INLINE_H

#include "pmc_sys.h"
#include "pmc_types.h"
#include "otu_frm_tx_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OTU_FRM_TX_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for otu_frm_tx
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
} otu_frm_tx_buffer_t;
static INLINE void otu_frm_tx_buffer_init( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_buffer_init( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "otu_frm_tx_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void otu_frm_tx_buffer_flush( otu_frm_tx_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_buffer_flush( otu_frm_tx_buffer_t *b_ptr )
{
    IOLOG( "otu_frm_tx_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 otu_frm_tx_reg_read( otu_frm_tx_buffer_t *b_ptr,
                                          otu_frm_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_read( otu_frm_tx_buffer_t *b_ptr,
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
static INLINE void otu_frm_tx_reg_write( otu_frm_tx_buffer_t *b_ptr,
                                         otu_frm_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_write( otu_frm_tx_buffer_t *b_ptr,
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

static INLINE void otu_frm_tx_field_set( otu_frm_tx_buffer_t *b_ptr,
                                         otu_frm_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 mask,
                                         UINT32 unused_mask,
                                         UINT32 ofs,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_set( otu_frm_tx_buffer_t *b_ptr,
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

static INLINE void otu_frm_tx_action_on_write_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_action_on_write_field_set( otu_frm_tx_buffer_t *b_ptr,
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

static INLINE void otu_frm_tx_burst_read( otu_frm_tx_buffer_t *b_ptr,
                                          otu_frm_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_burst_read( otu_frm_tx_buffer_t *b_ptr,
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

static INLINE void otu_frm_tx_burst_write( otu_frm_tx_buffer_t *b_ptr,
                                           otu_frm_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_burst_write( otu_frm_tx_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_poll( otu_frm_tx_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                    UINT32 max_count,
                                                    UINT32 *num_failed_polls,
                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_poll( otu_frm_tx_buffer_t *b_ptr,
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
 *  register access functions for otu_frm_tx
 * ==================================================================================
 */

static INLINE void otu_frm_tx_reg_OTU_SO_FAS_F6_write( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_FAS_F6_write( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_tx_reg_OTU_SO_FAS_F6_write", value );
    otu_frm_tx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_TX_REG_OTU_SO_FAS_F6,
                          value);
}

static INLINE void otu_frm_tx_reg_OTU_SO_FAS_F6_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_FAS_F6_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_tx_reg_OTU_SO_FAS_F6_field_set", mask, ofs, value );
    otu_frm_tx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_TX_REG_OTU_SO_FAS_F6,
                          mask,
                          PMC_OTU_FRM_TX_REG_OTU_SO_FAS_F6_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_tx_reg_OTU_SO_FAS_F6_read( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_OTU_SO_FAS_F6_read( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_tx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_OTU_FRM_TX_REG_OTU_SO_FAS_F6);

    IOLOG( "%s -> 0x%08x;", "otu_frm_tx_reg_OTU_SO_FAS_F6_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_tx_reg_OTU_SO_FAS_28_write( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_FAS_28_write( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_tx_reg_OTU_SO_FAS_28_write", value );
    otu_frm_tx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_TX_REG_OTU_SO_FAS_28,
                          value);
}

static INLINE void otu_frm_tx_reg_OTU_SO_FAS_28_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_FAS_28_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_tx_reg_OTU_SO_FAS_28_field_set", mask, ofs, value );
    otu_frm_tx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_TX_REG_OTU_SO_FAS_28,
                          mask,
                          PMC_OTU_FRM_TX_REG_OTU_SO_FAS_28_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_tx_reg_OTU_SO_FAS_28_read( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_OTU_SO_FAS_28_read( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_tx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_OTU_FRM_TX_REG_OTU_SO_FAS_28);

    IOLOG( "%s -> 0x%08x;", "otu_frm_tx_reg_OTU_SO_FAS_28_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_tx_reg_OTU_SO_CTRL_array_write( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_CTRL_array_write( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_tx_reg_OTU_SO_CTRL_array_write", value );
    otu_frm_tx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_TX_REG_OTU_SO_CTRL(N),
                          value);
}

static INLINE void otu_frm_tx_reg_OTU_SO_CTRL_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_CTRL_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_tx_reg_OTU_SO_CTRL_array_field_set", N, mask, ofs, value );
    otu_frm_tx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_TX_REG_OTU_SO_CTRL(N),
                          mask,
                          PMC_OTU_FRM_TX_REG_OTU_SO_CTRL_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_tx_reg_OTU_SO_CTRL_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_OTU_SO_CTRL_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_tx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_OTU_FRM_TX_REG_OTU_SO_CTRL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_tx_reg_OTU_SO_CTRL_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_write( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_write( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_write", value );
    otu_frm_tx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL(N),
                          value);
}

static INLINE void otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_field_set", N, mask, ofs, value );
    otu_frm_tx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL(N),
                          mask,
                          PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_tx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_tx_reg_OTU_SO_A_IAE_E_write( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_A_IAE_E_write( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_tx_reg_OTU_SO_A_IAE_E_write", value );
    otu_frm_tx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_E,
                          value);
}

static INLINE void otu_frm_tx_reg_OTU_SO_A_IAE_E_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_A_IAE_E_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_tx_reg_OTU_SO_A_IAE_E_field_set", mask, ofs, value );
    otu_frm_tx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_E,
                          mask,
                          PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_E_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_tx_reg_OTU_SO_A_IAE_E_read( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_OTU_SO_A_IAE_E_read( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_tx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_tx_reg_OTU_SO_A_IAE_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_tx_reg_OTU_SO_A_IAE_I_write( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_A_IAE_I_write( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_tx_reg_OTU_SO_A_IAE_I_write", value );
    otu_frm_tx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I,
                          value);
}

static INLINE void otu_frm_tx_reg_OTU_SO_A_IAE_I_action_on_write_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_A_IAE_I_action_on_write_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_tx_reg_OTU_SO_A_IAE_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_tx_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 otu_frm_tx_reg_OTU_SO_A_IAE_I_read( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_OTU_SO_A_IAE_I_read( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_tx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_tx_reg_OTU_SO_A_IAE_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_reg_OTU_SO_A_IAE_I_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_reg_OTU_SO_A_IAE_I_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_tx_reg_OTU_SO_A_IAE_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_tx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_tx_reg_OTU_SO_A_IAE_V_read( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_OTU_SO_A_IAE_V_read( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_tx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_tx_reg_OTU_SO_A_IAE_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_reg_OTU_SO_A_IAE_V_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_reg_OTU_SO_A_IAE_V_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_tx_reg_OTU_SO_A_IAE_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_tx_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_V,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_tx_reg_OTU_SO_MFAS_SM_array_write( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_MFAS_SM_array_write( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_tx_reg_OTU_SO_MFAS_SM_array_write", value );
    otu_frm_tx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM(N),
                          value);
}

static INLINE void otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set", N, mask, ofs, value );
    otu_frm_tx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM(N),
                          mask,
                          PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_tx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_tx_reg_OTU_SO_GCC0_RES_array_write( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_GCC0_RES_array_write( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_tx_reg_OTU_SO_GCC0_RES_array_write", value );
    otu_frm_tx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES(N),
                          value);
}

static INLINE void otu_frm_tx_reg_OTU_SO_GCC0_RES_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_GCC0_RES_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_tx_reg_OTU_SO_GCC0_RES_array_field_set", N, mask, ofs, value );
    otu_frm_tx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES(N),
                          mask,
                          PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_tx_reg_OTU_SO_GCC0_RES_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_OTU_SO_GCC0_RES_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_tx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_tx_reg_OTU_SO_GCC0_RES_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_write( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_write( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_write", value );
    otu_frm_tx_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL(N),
                          value);
}

static INLINE void otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set", N, mask, ofs, value );
    otu_frm_tx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL(N),
                          mask,
                          PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_tx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_write( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_write( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value )
{
    if (len + ofs > 16)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_write", ofs, len, 16 );
    IOLOG( "%s of %d words N=%d ofs=%d len=%d", "otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_write", 16, N, ofs, len);
    otu_frm_tx_burst_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI(N,ofs),
                            len,
                            value);

}

static INLINE void otu_frm_tx_reg_OTU_SO_SM_TTI_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 M,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_SM_TTI_array_field_set( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 M,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_tx_reg_OTU_SO_SM_TTI_array_field_set", N, mask, ofs, value );
    otu_frm_tx_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI(N,M),
                          mask,
                          PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE void otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_read( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_read( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 16)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_read", ofs, len, 16 );
    otu_frm_tx_burst_read( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI(N,ofs),
                           len,
                           value);

    IOLOG( "%s of %d words N=%d ofs=%d len=%d", "otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_read", 16, N, ofs, len);
}

static INLINE UINT32 otu_frm_tx_reg_OTU_SO_SM_TTI_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 M ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_reg_OTU_SO_SM_TTI_array_read( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 M )
{
    UINT32 reg_value;

    reg_value = otu_frm_tx_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI(N,M));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_tx_reg_OTU_SO_SM_TTI_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void otu_frm_tx_field_OTU_SO_FAS_F6_set( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_FAS_F6_set( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_FAS_F6_set", value, 16777215);
    IOLOG( "%s <= 0x%08x", "otu_frm_tx_field_OTU_SO_FAS_F6_set", value );

    /* (0x00003f04 bits 23:0) bits 0:23 use field OTU_SO_FAS_F6 of register PMC_OTU_FRM_TX_REG_OTU_SO_FAS_F6 */
    otu_frm_tx_reg_OTU_SO_FAS_F6_field_set( b_ptr,
                                            h_ptr,
                                            OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_MSK,
                                            OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_OFF,
                                            value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_FAS_F6_get( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_FAS_F6_get( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00003f04 bits 23:0) bits 0:23 use field OTU_SO_FAS_F6 of register PMC_OTU_FRM_TX_REG_OTU_SO_FAS_F6 */
    reg_value = otu_frm_tx_reg_OTU_SO_FAS_F6_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_MSK) >> OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_tx_field_OTU_SO_FAS_F6_get", value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_FAS_F6_set( otu_frm_tx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_FAS_F6_set( otu_frm_tx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_FAS_F6_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_FAS_F6_set", stop_bit, 23 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_FAS_F6_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00003f04 bits 23:0) bits 0:23 use field OTU_SO_FAS_F6 of register PMC_OTU_FRM_TX_REG_OTU_SO_FAS_F6 */
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
        /* (0x00003f04 bits 23:0) bits 0:23 use field OTU_SO_FAS_F6 of register PMC_OTU_FRM_TX_REG_OTU_SO_FAS_F6 */
        otu_frm_tx_reg_OTU_SO_FAS_F6_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_OFF + subfield_offset),
                                                OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_FAS_F6_get( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_FAS_F6_get( otu_frm_tx_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_FAS_F6_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_FAS_F6_get", stop_bit, 23 );
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
    /* (0x00003f04 bits 23:0) bits 0:23 use field OTU_SO_FAS_F6 of register PMC_OTU_FRM_TX_REG_OTU_SO_FAS_F6 */
    reg_value = otu_frm_tx_reg_OTU_SO_FAS_F6_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_MSK, OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_FAS_F6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_FAS_28_set( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_FAS_28_set( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_FAS_28_set", value, 16777215);
    IOLOG( "%s <= 0x%08x", "otu_frm_tx_field_OTU_SO_FAS_28_set", value );

    /* (0x00003f08 bits 23:0) bits 0:23 use field OTU_SO_FAS_28 of register PMC_OTU_FRM_TX_REG_OTU_SO_FAS_28 */
    otu_frm_tx_reg_OTU_SO_FAS_28_field_set( b_ptr,
                                            h_ptr,
                                            OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_MSK,
                                            OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_OFF,
                                            value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_FAS_28_get( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_FAS_28_get( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00003f08 bits 23:0) bits 0:23 use field OTU_SO_FAS_28 of register PMC_OTU_FRM_TX_REG_OTU_SO_FAS_28 */
    reg_value = otu_frm_tx_reg_OTU_SO_FAS_28_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_MSK) >> OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_tx_field_OTU_SO_FAS_28_get", value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_FAS_28_set( otu_frm_tx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_FAS_28_set( otu_frm_tx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_FAS_28_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_FAS_28_set", stop_bit, 23 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_FAS_28_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00003f08 bits 23:0) bits 0:23 use field OTU_SO_FAS_28 of register PMC_OTU_FRM_TX_REG_OTU_SO_FAS_28 */
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
        /* (0x00003f08 bits 23:0) bits 0:23 use field OTU_SO_FAS_28 of register PMC_OTU_FRM_TX_REG_OTU_SO_FAS_28 */
        otu_frm_tx_reg_OTU_SO_FAS_28_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_OFF + subfield_offset),
                                                OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_FAS_28_get( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_FAS_28_get( otu_frm_tx_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_FAS_28_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_FAS_28_get", stop_bit, 23 );
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
    /* (0x00003f08 bits 23:0) bits 0:23 use field OTU_SO_FAS_28 of register PMC_OTU_FRM_TX_REG_OTU_SO_FAS_28 */
    reg_value = otu_frm_tx_reg_OTU_SO_FAS_28_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_MSK, OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_FAS_28_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void otu_frm_tx_field_OTU_SO_MAINT_SIG_set( otu_frm_tx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_MAINT_SIG_set( otu_frm_tx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_MAINT_SIG_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_MAINT_SIG_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_MAINT_SIG_set", N, value );

    /* ((0x0000304c + (N) * 0x100) bits 2:1) bits 0:1 use field OTU_SO_MAINT_SIG of register PMC_OTU_FRM_TX_REG_OTU_SO_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_CTRL_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_MSK,
                                                OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_OFF,
                                                value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_MAINT_SIG_get( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_MAINT_SIG_get( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_MAINT_SIG_get", N, 11);
    /* ((0x0000304c + (N) * 0x100) bits 2:1) bits 0:1 use field OTU_SO_MAINT_SIG of register PMC_OTU_FRM_TX_REG_OTU_SO_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_CTRL_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_MSK) >> OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_MAINT_SIG_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_MAINT_SIG_set( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_MAINT_SIG_set( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_MAINT_SIG_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_MAINT_SIG_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_MAINT_SIG_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_MAINT_SIG_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000304c + (N) * 0x100) bits 2:1) bits 0:1 use field OTU_SO_MAINT_SIG of register PMC_OTU_FRM_TX_REG_OTU_SO_CTRL index N=0..11 */
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
        /* ((0x0000304c + (N) * 0x100) bits 2:1) bits 0:1 use field OTU_SO_MAINT_SIG of register PMC_OTU_FRM_TX_REG_OTU_SO_CTRL index N=0..11 */
        otu_frm_tx_reg_OTU_SO_CTRL_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_OFF + subfield_offset),
                                                    OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_MAINT_SIG_get( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_MAINT_SIG_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_MAINT_SIG_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_MAINT_SIG_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_MAINT_SIG_get", stop_bit, 1 );
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
    /* ((0x0000304c + (N) * 0x100) bits 2:1) bits 0:1 use field OTU_SO_MAINT_SIG of register PMC_OTU_FRM_TX_REG_OTU_SO_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_CTRL_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_MSK, OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_MAINT_SIG_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_set( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_set( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_set", N, value );

    /* ((0x0000304c + (N) * 0x100) bits 0) field OTU_SO_MI_ADMINSTATE of register PMC_OTU_FRM_TX_REG_OTU_SO_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_CTRL_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MI_ADMINSTATE_MSK,
                                                OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MI_ADMINSTATE_OFF,
                                                value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_get( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_get( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_get", N, 11);
    /* ((0x0000304c + (N) * 0x100) bits 0) field OTU_SO_MI_ADMINSTATE of register PMC_OTU_FRM_TX_REG_OTU_SO_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_CTRL_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MI_ADMINSTATE_MSK) >> OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MI_ADMINSTATE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_PASSTHRU_set( otu_frm_tx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_PASSTHRU_set( otu_frm_tx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_PASSTHRU_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_PASSTHRU_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_PASSTHRU_set", N, value );

    /* ((0x00003050 + (N) * 0x100) bits 0) field OTU_SO_PASSTHRU of register PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_BIT_OTU_SO_PASSTHRU_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_BIT_OTU_SO_PASSTHRU_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_PASSTHRU_get( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_PASSTHRU_get( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_PASSTHRU_get", N, 11);
    /* ((0x00003050 + (N) * 0x100) bits 0) field OTU_SO_PASSTHRU of register PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_BIT_OTU_SO_PASSTHRU_MSK) >> OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_BIT_OTU_SO_PASSTHRU_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_PASSTHRU_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void otu_frm_tx_field_OTU_SO_A_IAE_E_set( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_A_IAE_E_set( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_A_IAE_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_tx_field_OTU_SO_A_IAE_E_set", value );

    /* (0x00003f40 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_E of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_E */
    otu_frm_tx_reg_OTU_SO_A_IAE_E_field_set( b_ptr,
                                             h_ptr,
                                             OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_MSK,
                                             OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_OFF,
                                             value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_A_IAE_E_get( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_A_IAE_E_get( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00003f40 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_E of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_E */
    reg_value = otu_frm_tx_reg_OTU_SO_A_IAE_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_MSK) >> OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_tx_field_OTU_SO_A_IAE_E_get", value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_A_IAE_E_set( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_A_IAE_E_set( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_A_IAE_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_A_IAE_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_A_IAE_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00003f40 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_E of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_E */
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
        /* (0x00003f40 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_E of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_E */
        otu_frm_tx_reg_OTU_SO_A_IAE_E_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_OFF + subfield_offset),
                                                 OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_A_IAE_E_get( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_A_IAE_E_get( otu_frm_tx_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_A_IAE_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_A_IAE_E_get", stop_bit, 11 );
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
    /* (0x00003f40 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_E of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_E */
    reg_value = otu_frm_tx_reg_OTU_SO_A_IAE_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_MSK, OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_A_IAE_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void otu_frm_tx_field_OTU_SO_A_IAE_I_set_to_clear( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_A_IAE_I_set_to_clear( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_A_IAE_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_tx_field_OTU_SO_A_IAE_I_set_to_clear", value );

    /* (0x00003f80 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_I of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I */
    otu_frm_tx_reg_OTU_SO_A_IAE_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_MSK,
                                                             OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_OFF,
                                                             value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_A_IAE_I_get( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_A_IAE_I_get( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00003f80 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_I of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I */
    reg_value = otu_frm_tx_reg_OTU_SO_A_IAE_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_MSK) >> OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_tx_field_OTU_SO_A_IAE_I_get", value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_A_IAE_I_set_to_clear( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_A_IAE_I_set_to_clear( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_A_IAE_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_A_IAE_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_A_IAE_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00003f80 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_I of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I */
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
        /* (0x00003f80 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_I of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I */
        otu_frm_tx_reg_OTU_SO_A_IAE_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_OFF + subfield_offset),
                                                                 OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_A_IAE_I_get( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_A_IAE_I_get( otu_frm_tx_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_A_IAE_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_A_IAE_I_get", stop_bit, 11 );
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
    /* (0x00003f80 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_I of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I */
    reg_value = otu_frm_tx_reg_OTU_SO_A_IAE_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_MSK, OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_A_IAE_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_field_range_OTU_SO_A_IAE_I_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_field_range_OTU_SO_A_IAE_I_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_A_IAE_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_A_IAE_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_A_IAE_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00003f80 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_I of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I */
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
        /* (0x00003f80 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_I of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I */
        return otu_frm_tx_reg_OTU_SO_A_IAE_I_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_OFF + subfield_offset),
                                                   value << (OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_field_OTU_SO_A_IAE_I_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_field_OTU_SO_A_IAE_I_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_tx_field_OTU_SO_A_IAE_I_poll", value );

    /* (0x00003f80 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_I of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I */
    return otu_frm_tx_reg_OTU_SO_A_IAE_I_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_MSK,
                                               (value<<OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_OFF),
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
static INLINE UINT32 otu_frm_tx_field_OTU_SO_A_IAE_V_get( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_A_IAE_V_get( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00003fc0 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_V of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_V */
    reg_value = otu_frm_tx_reg_OTU_SO_A_IAE_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_MSK) >> OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_tx_field_OTU_SO_A_IAE_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_A_IAE_V_get( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_A_IAE_V_get( otu_frm_tx_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_A_IAE_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_A_IAE_V_get", stop_bit, 11 );
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
    /* (0x00003fc0 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_V of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_V */
    reg_value = otu_frm_tx_reg_OTU_SO_A_IAE_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_MSK, OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_A_IAE_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_field_range_OTU_SO_A_IAE_V_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_field_range_OTU_SO_A_IAE_V_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_A_IAE_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_A_IAE_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_A_IAE_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00003fc0 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_V of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_V */
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
        /* (0x00003fc0 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_V of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_V */
        return otu_frm_tx_reg_OTU_SO_A_IAE_V_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_OFF + subfield_offset),
                                                   value << (OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_field_OTU_SO_A_IAE_V_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_tx_field_OTU_SO_A_IAE_V_poll( otu_frm_tx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_tx_field_OTU_SO_A_IAE_V_poll", value );

    /* (0x00003fc0 bits 11:0) bits 0:11 use field OTU_SO_A_IAE_V of register PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_V */
    return otu_frm_tx_reg_OTU_SO_A_IAE_V_poll( b_ptr,
                                               h_ptr,
                                               OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_MSK,
                                               (value<<OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_OFF),
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
static INLINE void otu_frm_tx_field_OTU_SO_MFAS_set( otu_frm_tx_buffer_t *b_ptr,
                                                     otu_frm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_MFAS_set( otu_frm_tx_buffer_t *b_ptr,
                                                     otu_frm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_MFAS_set", N, 11);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_MFAS_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_MFAS_set", N, value );

    /* ((0x00003040 + (N) * 0x100) bits 23:16) field OTU_SO_MFAS of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_MSK,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_OFF,
                                                   value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_MFAS_get( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_MFAS_get( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_MFAS_get", N, 11);
    /* ((0x00003040 + (N) * 0x100) bits 23:16) field OTU_SO_MFAS of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_MSK) >> OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_MFAS_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_MFAS_set( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_MFAS_set( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_MFAS_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_MFAS_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_MFAS_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_MFAS_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003040 + (N) * 0x100) bits 23:16) field OTU_SO_MFAS of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
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
        /* ((0x00003040 + (N) * 0x100) bits 23:16) field OTU_SO_MFAS of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
        otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_OFF + subfield_offset),
                                                       OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_MFAS_get( otu_frm_tx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_MFAS_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_MFAS_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_MFAS_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_MFAS_get", stop_bit, 7 );
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
    /* ((0x00003040 + (N) * 0x100) bits 23:16) field OTU_SO_MFAS of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_MSK, OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_MFAS_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_BIP8_set( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_BIP8_set( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BIP8_set", N, 11);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BIP8_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BIP8_set", N, value );

    /* ((0x00003040 + (N) * 0x100) bits 15:8) field OTU_SO_SM_BIP8 of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_MSK,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_OFF,
                                                   value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BIP8_get( otu_frm_tx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BIP8_get( otu_frm_tx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BIP8_get", N, 11);
    /* ((0x00003040 + (N) * 0x100) bits 15:8) field OTU_SO_SM_BIP8 of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_MSK) >> OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BIP8_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_BIP8_set( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_BIP8_set( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003040 + (N) * 0x100) bits 15:8) field OTU_SO_SM_BIP8 of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
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
        /* ((0x00003040 + (N) * 0x100) bits 15:8) field OTU_SO_SM_BIP8 of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
        otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_OFF + subfield_offset),
                                                       OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_BIP8_get( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_BIP8_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_get", stop_bit, 7 );
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
    /* ((0x00003040 + (N) * 0x100) bits 15:8) field OTU_SO_SM_BIP8 of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_MSK, OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_set( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_set( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_set", N, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_set", N, value );

    /* ((0x00003040 + (N) * 0x100) bits 7:4) field OTU_SO_SM_BEI_BIAE of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_MSK,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_OFF,
                                                   value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_get( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_get( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_get", N, 11);
    /* ((0x00003040 + (N) * 0x100) bits 7:4) field OTU_SO_SM_BEI_BIAE of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_MSK) >> OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_set( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_set( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003040 + (N) * 0x100) bits 7:4) field OTU_SO_SM_BEI_BIAE of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
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
        /* ((0x00003040 + (N) * 0x100) bits 7:4) field OTU_SO_SM_BEI_BIAE of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
        otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_OFF + subfield_offset),
                                                       OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_get( otu_frm_tx_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_get", stop_bit, 3 );
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
    /* ((0x00003040 + (N) * 0x100) bits 7:4) field OTU_SO_SM_BEI_BIAE of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_MSK, OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_BDI_set( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_BDI_set( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BDI_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BDI_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BDI_set", N, value );

    /* ((0x00003040 + (N) * 0x100) bits 3) field OTU_SO_SM_BDI of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BDI_MSK,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BDI_OFF,
                                                   value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BDI_get( otu_frm_tx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BDI_get( otu_frm_tx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BDI_get", N, 11);
    /* ((0x00003040 + (N) * 0x100) bits 3) field OTU_SO_SM_BDI of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BDI_MSK) >> OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BDI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BDI_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_IAE_set( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_IAE_set( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_IAE_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_IAE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_IAE_set", N, value );

    /* ((0x00003040 + (N) * 0x100) bits 2) field OTU_SO_SM_IAE of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_IAE_MSK,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_IAE_OFF,
                                                   value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_IAE_get( otu_frm_tx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_IAE_get( otu_frm_tx_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_IAE_get", N, 11);
    /* ((0x00003040 + (N) * 0x100) bits 2) field OTU_SO_SM_IAE of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_IAE_MSK) >> OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_IAE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_IAE_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_RES7_set( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_RES7_set( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_RES7_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES7_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES7_set", N, value );

    /* ((0x00003040 + (N) * 0x100) bits 1) field OTU_SO_SM_RES7 of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES7_MSK,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES7_OFF,
                                                   value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_RES7_get( otu_frm_tx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_RES7_get( otu_frm_tx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_RES7_get", N, 11);
    /* ((0x00003040 + (N) * 0x100) bits 1) field OTU_SO_SM_RES7 of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES7_MSK) >> OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES7_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES7_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_RES8_set( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_RES8_set( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_RES8_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES8_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES8_set", N, value );

    /* ((0x00003040 + (N) * 0x100) bits 0) field OTU_SO_SM_RES8 of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES8_MSK,
                                                   OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES8_OFF,
                                                   value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_RES8_get( otu_frm_tx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_RES8_get( otu_frm_tx_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_RES8_get", N, 11);
    /* ((0x00003040 + (N) * 0x100) bits 0) field OTU_SO_SM_RES8 of register PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES8_MSK) >> OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES8_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES8_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_GCC0_set( otu_frm_tx_buffer_t *b_ptr,
                                                     otu_frm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_GCC0_set( otu_frm_tx_buffer_t *b_ptr,
                                                     otu_frm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_GCC0_set", N, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_GCC0_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_GCC0_set", N, value );

    /* ((0x00003044 + (N) * 0x100) bits 31:16) field OTU_SO_GCC0 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
    otu_frm_tx_reg_OTU_SO_GCC0_RES_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_MSK,
                                                    OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_GCC0_get( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_GCC0_get( otu_frm_tx_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_GCC0_get", N, 11);
    /* ((0x00003044 + (N) * 0x100) bits 31:16) field OTU_SO_GCC0 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_GCC0_RES_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_MSK) >> OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_GCC0_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_GCC0_set( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_GCC0_set( otu_frm_tx_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_GCC0_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_GCC0_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_GCC0_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_GCC0_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003044 + (N) * 0x100) bits 31:16) field OTU_SO_GCC0 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
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
        /* ((0x00003044 + (N) * 0x100) bits 31:16) field OTU_SO_GCC0 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
        otu_frm_tx_reg_OTU_SO_GCC0_RES_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_OFF + subfield_offset),
                                                        OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_GCC0_get( otu_frm_tx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_GCC0_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_GCC0_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_GCC0_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_GCC0_get", stop_bit, 15 );
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
    /* ((0x00003044 + (N) * 0x100) bits 31:16) field OTU_SO_GCC0 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_GCC0_RES_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_MSK, OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_GCC0_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_RES13_set( otu_frm_tx_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_RES13_set( otu_frm_tx_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_RES13_set", N, 11);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_RES13_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_RES13_set", N, value );

    /* ((0x00003044 + (N) * 0x100) bits 15:8) field OTU_SO_RES13 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
    otu_frm_tx_reg_OTU_SO_GCC0_RES_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_MSK,
                                                    OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_RES13_get( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_RES13_get( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_RES13_get", N, 11);
    /* ((0x00003044 + (N) * 0x100) bits 15:8) field OTU_SO_RES13 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_GCC0_RES_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_MSK) >> OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_RES13_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_RES13_set( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_RES13_set( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_RES13_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_RES13_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_RES13_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_RES13_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003044 + (N) * 0x100) bits 15:8) field OTU_SO_RES13 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
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
        /* ((0x00003044 + (N) * 0x100) bits 15:8) field OTU_SO_RES13 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
        otu_frm_tx_reg_OTU_SO_GCC0_RES_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_OFF + subfield_offset),
                                                        OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_RES13_get( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_RES13_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_RES13_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_RES13_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_RES13_get", stop_bit, 7 );
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
    /* ((0x00003044 + (N) * 0x100) bits 15:8) field OTU_SO_RES13 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_GCC0_RES_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_MSK, OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_RES13_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_RES14_set( otu_frm_tx_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_RES14_set( otu_frm_tx_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_RES14_set", N, 11);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_RES14_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_RES14_set", N, value );

    /* ((0x00003044 + (N) * 0x100) bits 7:0) field OTU_SO_RES14 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
    otu_frm_tx_reg_OTU_SO_GCC0_RES_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_MSK,
                                                    OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_RES14_get( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_RES14_get( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_RES14_get", N, 11);
    /* ((0x00003044 + (N) * 0x100) bits 7:0) field OTU_SO_RES14 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_GCC0_RES_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_MSK) >> OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_RES14_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_RES14_set( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_RES14_set( otu_frm_tx_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_RES14_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_RES14_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_RES14_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_RES14_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003044 + (N) * 0x100) bits 7:0) field OTU_SO_RES14 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
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
        /* ((0x00003044 + (N) * 0x100) bits 7:0) field OTU_SO_RES14 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
        otu_frm_tx_reg_OTU_SO_GCC0_RES_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_OFF + subfield_offset),
                                                        OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_RES14_get( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_RES14_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_RES14_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_RES14_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_RES14_get", stop_bit, 7 );
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
    /* ((0x00003044 + (N) * 0x100) bits 7:0) field OTU_SO_RES14 of register PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_GCC0_RES_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_MSK, OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_RES14_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 19) field OTU_SO_FAS_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_FAS_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_FAS_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 19) field OTU_SO_FAS_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_FAS_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_FAS_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 18:17) field OTU_SO_MFAS_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 18:17) field OTU_SO_MFAS_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003048 + (N) * 0x100) bits 18:17) field OTU_SO_MFAS_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
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
        /* ((0x00003048 + (N) * 0x100) bits 18:17) field OTU_SO_MFAS_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
        otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_OFF + subfield_offset),
                                                           OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_get", stop_bit, 1 );
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
    /* ((0x00003048 + (N) * 0x100) bits 18:17) field OTU_SO_MFAS_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_MSK, OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 16) field OTU_SO_SM_TTI_SAPI_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_SAPI_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_SAPI_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 16) field OTU_SO_SM_TTI_SAPI_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_SAPI_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_SAPI_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 15) field OTU_SO_SM_TTI_DAPI_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_DAPI_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_DAPI_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 15) field OTU_SO_SM_TTI_DAPI_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_DAPI_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_DAPI_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 14) field OTU_SO_SM_TTI_OP_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_OP_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_OP_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 14) field OTU_SO_SM_TTI_OP_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_OP_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_OP_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_set", N, 11);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 13:11) field OTU_SO_SM_BIP8_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 13:11) field OTU_SO_SM_BIP8_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003048 + (N) * 0x100) bits 13:11) field OTU_SO_SM_BIP8_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
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
        /* ((0x00003048 + (N) * 0x100) bits 13:11) field OTU_SO_SM_BIP8_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
        otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_OFF + subfield_offset),
                                                           OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                         otu_frm_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_get", stop_bit, 2 );
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
    /* ((0x00003048 + (N) * 0x100) bits 13:11) field OTU_SO_SM_BIP8_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_MSK, OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 10:9) field OTU_SO_SM_BEI_BIAE_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 10:9) field OTU_SO_SM_BEI_BIAE_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003048 + (N) * 0x100) bits 10:9) field OTU_SO_SM_BEI_BIAE_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
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
        /* ((0x00003048 + (N) * 0x100) bits 10:9) field OTU_SO_SM_BEI_BIAE_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
        otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_OFF + subfield_offset),
                                                           OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_get", stop_bit, 1 );
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
    /* ((0x00003048 + (N) * 0x100) bits 10:9) field OTU_SO_SM_BEI_BIAE_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_MSK, OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 8:7) field OTU_SO_SM_BDI_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 8:7) field OTU_SO_SM_BDI_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003048 + (N) * 0x100) bits 8:7) field OTU_SO_SM_BDI_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
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
        /* ((0x00003048 + (N) * 0x100) bits 8:7) field OTU_SO_SM_BDI_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
        otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_OFF + subfield_offset),
                                                           OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_get", stop_bit, 1 );
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
    /* ((0x00003048 + (N) * 0x100) bits 8:7) field OTU_SO_SM_BDI_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_MSK, OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 6:5) field OTU_SO_SM_IAE_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 6:5) field OTU_SO_SM_IAE_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003048 + (N) * 0x100) bits 6:5) field OTU_SO_SM_IAE_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
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
        /* ((0x00003048 + (N) * 0x100) bits 6:5) field OTU_SO_SM_IAE_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
        otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_OFF + subfield_offset),
                                                           OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_get", stop_bit, 1 );
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
    /* ((0x00003048 + (N) * 0x100) bits 6:5) field OTU_SO_SM_IAE_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_MSK)
                  >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_MSK, OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 4) field OTU_SO_SM_RES7_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES7_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES7_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 4) field OTU_SO_SM_RES7_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES7_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES7_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 3) field OTU_SO_SM_RES8_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES8_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES8_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 3) field OTU_SO_SM_RES8_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES8_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES8_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 2) field OTU_SO_GCC0_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_GCC0_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_GCC0_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 2) field OTU_SO_GCC0_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_GCC0_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_GCC0_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 1) field OTU_SO_RES13_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES13_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES13_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 1) field OTU_SO_RES13_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES13_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES13_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_get", N, value );

    return value;
}
static INLINE void otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_set( otu_frm_tx_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_set", N, value );

    /* ((0x00003048 + (N) * 0x100) bits 0) field OTU_SO_RES14_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES14_INS_CTRL_MSK,
                                                       OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES14_INS_CTRL_OFF,
                                                       value);
}

static INLINE UINT32 otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_get( otu_frm_tx_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_get", N, 11);
    /* ((0x00003048 + (N) * 0x100) bits 0) field OTU_SO_RES14_INS_CTRL of register PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL index N=0..11 */
    reg_value = otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES14_INS_CTRL_MSK) >> OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES14_INS_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size12__M_burstsize16
 * ==================================================================================
 */
static INLINE void otu_frm_tx_lfield_OTU_SO_SM_TTI_set( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value[16] ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_lfield_OTU_SO_SM_TTI_set( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value[16] )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_lfield_OTU_SO_SM_TTI_set", N, 11);
    IOLOG( "%s  N=%d", "otu_frm_tx_lfield_OTU_SO_SM_TTI_set", N);
    otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_write( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    0,
                                                    16,
                                                    value);
}

static INLINE void otu_frm_tx_lfield_OTU_SO_SM_TTI_get( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value[16] ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_lfield_OTU_SO_SM_TTI_get( otu_frm_tx_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value[16] )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_lfield_OTU_SO_SM_TTI_get", N, 11);
    IOLOG( "%s  N=%d", "otu_frm_tx_lfield_OTU_SO_SM_TTI_get", N);
    otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_read( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   0,
                                                   16,
                                                   (UINT32 *)value);
}

static INLINE void otu_frm_tx_lfield_range_OTU_SO_SM_TTI_set( otu_frm_tx_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_tx_lfield_range_OTU_SO_SM_TTI_set( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_lfield_range_OTU_SO_SM_TTI_set", N, 11);
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "otu_frm_tx_lfield_range_OTU_SO_SM_TTI_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d value=0x%08x", "otu_frm_tx_lfield_range_OTU_SO_SM_TTI_set", N, start_bit, stop_bit, value);
    otu_frm_tx_reg_OTU_SO_SM_TTI_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  word_number,
                                                  mask,
                                                  field_ofs,
                                                  value);
}

static INLINE UINT32 otu_frm_tx_lfield_range_OTU_SO_SM_TTI_get( otu_frm_tx_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 otu_frm_tx_lfield_range_OTU_SO_SM_TTI_get( otu_frm_tx_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_tx_lfield_range_OTU_SO_SM_TTI_get", N, 11);
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "otu_frm_tx_lfield_range_OTU_SO_SM_TTI_get", start_bit, stop_bit );
    }
    reg_value = otu_frm_tx_reg_OTU_SO_SM_TTI_array_read( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d value=0x%08x", "otu_frm_tx_lfield_range_OTU_SO_SM_TTI_get", N, start_bit, stop_bit, value );
    return value;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OTU_FRM_TX_IO_INLINE_H */
