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
 *     and register accessor functions for the otu_frm block
 *****************************************************************************/
#ifndef _OTU_FRM_IO_INLINE_H
#define _OTU_FRM_IO_INLINE_H

#include "otu_frm_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "otu_frm.h"
#include "otu_frm_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OTU_FRM_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for otu_frm
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
} otu_frm_buffer_t;
static INLINE void otu_frm_buffer_init( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void otu_frm_buffer_init( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "otu_frm_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void otu_frm_buffer_flush( otu_frm_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void otu_frm_buffer_flush( otu_frm_buffer_t *b_ptr )
{
    IOLOG( "otu_frm_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 otu_frm_reg_read( otu_frm_buffer_t *b_ptr,
                                       otu_frm_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_read( otu_frm_buffer_t *b_ptr,
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
static INLINE void otu_frm_reg_write( otu_frm_buffer_t *b_ptr,
                                      otu_frm_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_write( otu_frm_buffer_t *b_ptr,
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

static INLINE void otu_frm_field_set( otu_frm_buffer_t *b_ptr,
                                      otu_frm_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_set( otu_frm_buffer_t *b_ptr,
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

static INLINE void otu_frm_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
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

static INLINE void otu_frm_burst_read( otu_frm_buffer_t *b_ptr,
                                       otu_frm_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void otu_frm_burst_read( otu_frm_buffer_t *b_ptr,
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

static INLINE void otu_frm_burst_write( otu_frm_buffer_t *b_ptr,
                                        otu_frm_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void otu_frm_burst_write( otu_frm_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE otu_frm_poll( otu_frm_buffer_t *b_ptr,
                                                 otu_frm_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_poll( otu_frm_buffer_t *b_ptr,
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
 *  register access functions for otu_frm
 * ==================================================================================
 */

static INLINE void otu_frm_reg_OTU_FRM_MASTER_CFG_write( otu_frm_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_MASTER_CFG_write( otu_frm_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_MASTER_CFG_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_MASTER_CFG_field_set( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_MASTER_CFG_field_set( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_MASTER_CFG_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_MASTER_CFG_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_MASTER_CFG_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_MASTER_CFG_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_SW_PMON_write( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_SW_PMON_write( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_SW_PMON_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_SW_PMON,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_SW_PMON_field_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_SW_PMON_field_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_SW_PMON_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_SW_PMON,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_SW_PMON_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_SW_PMON_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_SW_PMON_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTU_FRM_REG_OTU_FRM_SW_PMON);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_SW_PMON_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_SW_RESET_write( otu_frm_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_SW_RESET_write( otu_frm_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_SW_RESET_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_SW_RESET,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_SW_RESET_field_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_SW_RESET_field_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_SW_RESET_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_SW_RESET,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_SW_RESET_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_SW_RESET_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_SW_RESET_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTU_FRM_REG_OTU_FRM_SW_RESET);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_SW_RESET_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_write( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_write( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_field_set( otu_frm_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_field_set( otu_frm_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_write( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_write( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_100G_ENABLE_write( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_100G_ENABLE_write( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_100G_ENABLE_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_100G_ENABLE_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_100G_ENABLE_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_100G_ENABLE_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_100G_ENABLE_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_SPARE_write( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_SPARE_write( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_SPARE_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_SPARE,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_SPARE_field_set( otu_frm_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_SPARE_field_set( otu_frm_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_SPARE_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_SPARE,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_SPARE_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_SPARE_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_SPARE_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTU_FRM_REG_OTU_FRM_SPARE);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_SPARE_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_10G_ENABLE_array_write( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_10G_ENABLE_array_write( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_10G_ENABLE_array_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE(N),
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set", N, mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE(N),
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_reg_OTU_FRM_10G_ENABLE_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_STREAM_CFG_array_write( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_STREAM_CFG_array_write( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_STREAM_CFG_array_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG(N),
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_STREAM_CFG_array_field_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_STREAM_CFG_array_field_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_STREAM_CFG_array_field_set", N, mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG(N),
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_STREAM_CFG_array_read( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_STREAM_CFG_array_read( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_reg_OTU_FRM_STREAM_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_40G_ENABLE_array_write( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_40G_ENABLE_array_write( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_40G_ENABLE_array_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE(N),
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set", N, mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE(N),
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otu_frm_reg_OTU_FRM_40G_ENABLE_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_INTLV_WOVR_E_write( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_INTLV_WOVR_E_write( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_INTLV_WOVR_E_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_INTLV_WOVR_E_field_set( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_INTLV_WOVR_E_field_set( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_INTLV_WOVR_E_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_WOVR_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_WOVR_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_INTLV_WOVR_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_INTLV_RUDR_E_write( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_INTLV_RUDR_E_write( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_INTLV_RUDR_E_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_INTLV_RUDR_E_field_set( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_INTLV_RUDR_E_field_set( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_INTLV_RUDR_E_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_RUDR_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_RUDR_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_INTLV_RUDR_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_write( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_write( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_field_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_field_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_write( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_write( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_field_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_field_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_D_LOM_E_write( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_D_LOM_E_write( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_D_LOM_E_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_D_LOM_E,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_D_LOM_E_field_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_D_LOM_E_field_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_D_LOM_E_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_D_LOM_E,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_D_LOM_E_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_D_LOM_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_D_LOM_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_D_LOM_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_D_LOM_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_OOM_E_write( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_OOM_E_write( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_OOM_E_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_OOM_E,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_OOM_E_field_set( otu_frm_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_OOM_E_field_set( otu_frm_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_OOM_E_field_set", mask, ofs, value );
    otu_frm_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_OOM_E,
                       mask,
                       PMC_OTU_FRM_REG_OTU_FRM_OOM_E_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_OOM_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_OOM_E_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_OOM_E);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_OOM_E_read", reg_value);
    return reg_value;
}

static INLINE void otu_frm_reg_OTU_FRM_INTLV_WOVR_I_write( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_INTLV_WOVR_I_write( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_INTLV_WOVR_I_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_INTLV_WOVR_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_INTLV_WOVR_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_INTLV_WOVR_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_WOVR_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_WOVR_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_INTLV_WOVR_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_INTLV_WOVR_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_reg_OTU_FRM_INTLV_RUDR_I_write( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_INTLV_RUDR_I_write( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_INTLV_RUDR_I_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_INTLV_RUDR_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_INTLV_RUDR_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                               otu_frm_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_INTLV_RUDR_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_RUDR_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_RUDR_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_INTLV_RUDR_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_INTLV_RUDR_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_write( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_write( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_write( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_write( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                                otu_frm_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_reg_OTU_FRM_D_LOM_I_write( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_D_LOM_I_write( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_D_LOM_I_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_D_LOM_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_D_LOM_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_D_LOM_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_D_LOM_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_D_LOM_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_D_LOM_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_D_LOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_D_LOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_D_LOM_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void otu_frm_reg_OTU_FRM_OOM_I_write( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_OOM_I_write( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otu_frm_reg_OTU_FRM_OOM_I_write", value );
    otu_frm_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTU_FRM_REG_OTU_FRM_OOM_I,
                       value);
}

static INLINE void otu_frm_reg_OTU_FRM_OOM_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_reg_OTU_FRM_OOM_I_action_on_write_field_set( otu_frm_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otu_frm_reg_OTU_FRM_OOM_I_action_on_write_field_set", mask, ofs, value );
    otu_frm_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OTU_FRM_REG_OTU_FRM_OOM_I,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 otu_frm_reg_OTU_FRM_OOM_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_OOM_I_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_OOM_I);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_OOM_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_OOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_OOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_OOM_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_OOM_I,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_WOVR_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_WOVR_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_INTLV_WOVR_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_INTLV_WOVR_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_RUDR_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_INTLV_RUDR_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_INTLV_RUDR_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_INTLV_RUDR_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_reg_OTU_FRM_D_LOM_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_D_LOM_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_D_LOM_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_D_LOM_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_D_LOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_D_LOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_D_LOM_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_D_LOM_V,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_reg_OTU_FRM_OOM_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_OOM_V_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_OOM_V);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_OOM_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_OOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_OOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otu_frm_reg_OTU_FRM_OOM_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otu_frm_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTU_FRM_REG_OTU_FRM_OOM_V,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 otu_frm_reg_OTU_FRM_INT_SUMMARY_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_reg_OTU_FRM_INT_SUMMARY_read( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otu_frm_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTU_FRM_REG_OTU_FRM_INT_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "otu_frm_reg_OTU_FRM_INT_SUMMARY_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void otu_frm_field_OTU_FRM_SW_RESET_set( otu_frm_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_SW_RESET_set( otu_frm_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_SW_RESET_set", value );

    /* (0x00000000 bits 31) field OTU_FRM_SW_RESET of register PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG */
    otu_frm_reg_OTU_FRM_MASTER_CFG_field_set( b_ptr,
                                              h_ptr,
                                              OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RESET_MSK,
                                              OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RESET_OFF,
                                              value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_SW_RESET_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_SW_RESET_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 31) field OTU_FRM_SW_RESET of register PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG */
    reg_value = otu_frm_reg_OTU_FRM_MASTER_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RESET_MSK) >> OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_SW_RESET_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_SW_RAM_LOWPWR_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_SW_RAM_LOWPWR_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_SW_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_SW_RAM_LOWPWR_set", value );

    /* (0x00000000 bits 1) field OTU_FRM_SW_RAM_LOWPWR of register PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG */
    otu_frm_reg_OTU_FRM_MASTER_CFG_field_set( b_ptr,
                                              h_ptr,
                                              OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RAM_LOWPWR_MSK,
                                              OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RAM_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_SW_RAM_LOWPWR_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_SW_RAM_LOWPWR_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field OTU_FRM_SW_RAM_LOWPWR of register PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG */
    reg_value = otu_frm_reg_OTU_FRM_MASTER_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RAM_LOWPWR_MSK) >> OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_SW_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_SW_LOWPWR_set( otu_frm_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_SW_LOWPWR_set( otu_frm_buffer_t *b_ptr,
                                                        otu_frm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_SW_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_SW_LOWPWR_set", value );

    /* (0x00000000 bits 0) field OTU_FRM_SW_LOWPWR of register PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG */
    otu_frm_reg_OTU_FRM_MASTER_CFG_field_set( b_ptr,
                                              h_ptr,
                                              OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_LOWPWR_MSK,
                                              OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_SW_LOWPWR_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_SW_LOWPWR_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field OTU_FRM_SW_LOWPWR of register PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG */
    reg_value = otu_frm_reg_OTU_FRM_MASTER_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_LOWPWR_MSK) >> OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_SW_LOWPWR_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_LCLK_EN_set( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_LCLK_EN_set( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_LCLK_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_LCLK_EN_set", value );

    /* (0x00000004 bits 1) field OTU_FRM_LCLK_EN of register PMC_OTU_FRM_REG_OTU_FRM_SW_PMON */
    otu_frm_reg_OTU_FRM_SW_PMON_field_set( b_ptr,
                                           h_ptr,
                                           OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_LCLK_EN_MSK,
                                           OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_LCLK_EN_OFF,
                                           value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_LCLK_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_LCLK_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 1) field OTU_FRM_LCLK_EN of register PMC_OTU_FRM_REG_OTU_FRM_SW_PMON */
    reg_value = otu_frm_reg_OTU_FRM_SW_PMON_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_LCLK_EN_MSK) >> OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_LCLK_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_LCLK_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_SW_SYSCLK_RESET_set( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_SW_SYSCLK_RESET_set( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_SW_SYSCLK_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_SW_SYSCLK_RESET_set", value );

    /* (0x0000000c bits 13) field OTU_FRM_SW_SYSCLK_RESET of register PMC_OTU_FRM_REG_OTU_FRM_SW_RESET */
    otu_frm_reg_OTU_FRM_SW_RESET_field_set( b_ptr,
                                            h_ptr,
                                            OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_SYSCLK_RESET_MSK,
                                            OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_SYSCLK_RESET_OFF,
                                            value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_SW_SYSCLK_RESET_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_SW_SYSCLK_RESET_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 13) field OTU_FRM_SW_SYSCLK_RESET of register PMC_OTU_FRM_REG_OTU_FRM_SW_RESET */
    reg_value = otu_frm_reg_OTU_FRM_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_SYSCLK_RESET_MSK) >> OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_SYSCLK_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_SW_SYSCLK_RESET_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_SW_CLK355M_RESET_set( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_SW_CLK355M_RESET_set( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_SW_CLK355M_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_SW_CLK355M_RESET_set", value );

    /* (0x0000000c bits 12) field OTU_FRM_SW_CLK355M_RESET of register PMC_OTU_FRM_REG_OTU_FRM_SW_RESET */
    otu_frm_reg_OTU_FRM_SW_RESET_field_set( b_ptr,
                                            h_ptr,
                                            OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_CLK355M_RESET_MSK,
                                            OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_CLK355M_RESET_OFF,
                                            value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_SW_CLK355M_RESET_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_SW_CLK355M_RESET_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 12) field OTU_FRM_SW_CLK355M_RESET of register PMC_OTU_FRM_REG_OTU_FRM_SW_RESET */
    reg_value = otu_frm_reg_OTU_FRM_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_CLK355M_RESET_MSK) >> OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_CLK355M_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_SW_CLK355M_RESET_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_SW_RX_LINECLK_RESET_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_SW_RX_LINECLK_RESET_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_SW_RX_LINECLK_RESET_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_SW_RX_LINECLK_RESET_set", value );

    /* (0x0000000c bits 11:0) bits 0:11 use field OTU_FRM_SW_RX_LINECLK_RESET of register PMC_OTU_FRM_REG_OTU_FRM_SW_RESET */
    otu_frm_reg_OTU_FRM_SW_RESET_field_set( b_ptr,
                                            h_ptr,
                                            OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_MSK,
                                            OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_OFF,
                                            value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_SW_RX_LINECLK_RESET_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_SW_RX_LINECLK_RESET_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 11:0) bits 0:11 use field OTU_FRM_SW_RX_LINECLK_RESET of register PMC_OTU_FRM_REG_OTU_FRM_SW_RESET */
    reg_value = otu_frm_reg_OTU_FRM_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_MSK) >> OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_SW_RX_LINECLK_RESET_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_set( otu_frm_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_set( otu_frm_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 11:0) bits 0:11 use field OTU_FRM_SW_RX_LINECLK_RESET of register PMC_OTU_FRM_REG_OTU_FRM_SW_RESET */
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
        /* (0x0000000c bits 11:0) bits 0:11 use field OTU_FRM_SW_RX_LINECLK_RESET of register PMC_OTU_FRM_REG_OTU_FRM_SW_RESET */
        otu_frm_reg_OTU_FRM_SW_RESET_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_OFF + subfield_offset),
                                                OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_get( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_get", stop_bit, 11 );
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
    /* (0x0000000c bits 11:0) bits 0:11 use field OTU_FRM_SW_RX_LINECLK_RESET of register PMC_OTU_FRM_REG_OTU_FRM_SW_RESET */
    reg_value = otu_frm_reg_OTU_FRM_SW_RESET_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_MSK)
                  >> OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_MSK, OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_SF_CNT_LIMIT_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_SF_CNT_LIMIT_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_SF_CNT_LIMIT_set", value );

    /* (0x00000018 bits 31:0) bits 0:31 use field OTU_FRM_SF_CNT_LIMIT of register PMC_OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT */
    otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_field_set( b_ptr,
                                                        h_ptr,
                                                        OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_MSK,
                                                        OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_OFF,
                                                        value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_SF_CNT_LIMIT_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_SF_CNT_LIMIT_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 31:0) bits 0:31 use field OTU_FRM_SF_CNT_LIMIT of register PMC_OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT */
    reg_value = otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_MSK) >> OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_SF_CNT_LIMIT_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 31:0) bits 0:31 use field OTU_FRM_SF_CNT_LIMIT of register PMC_OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000018 bits 31:0) bits 0:31 use field OTU_FRM_SF_CNT_LIMIT of register PMC_OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT */
        otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_OFF + subfield_offset),
                                                            OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_get( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_get", stop_bit, 31 );
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
    /* (0x00000018 bits 31:0) bits 0:31 use field OTU_FRM_SF_CNT_LIMIT of register PMC_OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT */
    reg_value = otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_MSK)
                  >> OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_MSK, OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_set( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_set( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_set", value );

    /* (0x0000001c bits 22) field OTU_FRM_RX_LINECLK_11_SRC2_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC2_EN_MSK,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC2_EN_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 22) field OTU_FRM_RX_LINECLK_11_SRC2_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    reg_value = otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC2_EN_MSK) >> OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC2_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_set( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_set( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_set", value );

    /* (0x0000001c bits 21) field OTU_FRM_RX_LINECLK_11_SRC1_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC1_EN_MSK,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC1_EN_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 21) field OTU_FRM_RX_LINECLK_11_SRC1_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    reg_value = otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC1_EN_MSK) >> OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC1_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_set( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_set( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_set", value );

    /* (0x0000001c bits 20) field OTU_FRM_RX_LINECLK_11_SRC0_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC0_EN_MSK,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC0_EN_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 20) field OTU_FRM_RX_LINECLK_11_SRC0_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    reg_value = otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC0_EN_MSK) >> OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC0_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_set", value );

    /* (0x0000001c bits 19) field OTU_FRM_RX_LINECLK_7_SRC1_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC1_EN_MSK,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC1_EN_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 19) field OTU_FRM_RX_LINECLK_7_SRC1_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    reg_value = otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC1_EN_MSK) >> OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC1_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_set", value );

    /* (0x0000001c bits 18) field OTU_FRM_RX_LINECLK_7_SRC0_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC0_EN_MSK,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC0_EN_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 18) field OTU_FRM_RX_LINECLK_7_SRC0_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    reg_value = otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC0_EN_MSK) >> OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC0_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_set", value );

    /* (0x0000001c bits 17) field OTU_FRM_RX_LINECLK_3_SRC1_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC1_EN_MSK,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC1_EN_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 17) field OTU_FRM_RX_LINECLK_3_SRC1_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    reg_value = otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC1_EN_MSK) >> OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC1_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_set", value );

    /* (0x0000001c bits 16) field OTU_FRM_RX_LINECLK_3_SRC0_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC0_EN_MSK,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC0_EN_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 16) field OTU_FRM_RX_LINECLK_3_SRC0_EN of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    reg_value = otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC0_EN_MSK) >> OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC0_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_11_SEL_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_11_SEL_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SEL_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SEL_set", value );

    /* (0x0000001c bits 3:2) bits 0:1 use field OTU_FRM_RX_LINECLK_11_SEL of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_MSK,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SEL_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SEL_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 3:2) bits 0:1 use field OTU_FRM_RX_LINECLK_11_SEL of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    reg_value = otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_MSK) >> OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_11_SEL_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_set( otu_frm_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_set( otu_frm_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000001c bits 3:2) bits 0:1 use field OTU_FRM_RX_LINECLK_11_SEL of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
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
        /* (0x0000001c bits 3:2) bits 0:1 use field OTU_FRM_RX_LINECLK_11_SEL of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
        otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_OFF + subfield_offset),
                                                          OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_get( otu_frm_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_get", stop_bit, 1 );
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
    /* (0x0000001c bits 3:2) bits 0:1 use field OTU_FRM_RX_LINECLK_11_SEL of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    reg_value = otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_MSK)
                  >> OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_MSK, OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_7_SEL_set( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_7_SEL_set( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_7_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_7_SEL_set", value );

    /* (0x0000001c bits 1) field OTU_FRM_RX_LINECLK_7_SEL of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SEL_MSK,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SEL_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_7_SEL_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_7_SEL_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 1) field OTU_FRM_RX_LINECLK_7_SEL of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    reg_value = otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SEL_MSK) >> OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_7_SEL_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_3_SEL_set( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_RX_LINECLK_3_SEL_set( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_3_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_3_SEL_set", value );

    /* (0x0000001c bits 0) field OTU_FRM_RX_LINECLK_3_SEL of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SEL_MSK,
                                                      OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SEL_OFF,
                                                      value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_3_SEL_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_3_SEL_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 0) field OTU_FRM_RX_LINECLK_3_SEL of register PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG */
    reg_value = otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SEL_MSK) >> OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_RX_LINECLK_3_SEL_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_set", value );

    /* (0x0000007c bits 10) field OTU_FRM_100G_SCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( b_ptr,
                                               h_ptr,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SCRAMBLING_EN_MSK,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SCRAMBLING_EN_OFF,
                                               value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000007c bits 10) field OTU_FRM_100G_SCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    reg_value = otu_frm_reg_OTU_FRM_100G_ENABLE_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SCRAMBLING_EN_MSK) >> OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SCRAMBLING_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_100G_FEC_COL_INS_EN_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_100G_FEC_COL_INS_EN_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_100G_FEC_COL_INS_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_100G_FEC_COL_INS_EN_set", value );

    /* (0x0000007c bits 9) field OTU_FRM_100G_FEC_COL_INS_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( b_ptr,
                                               h_ptr,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_INS_EN_MSK,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_INS_EN_OFF,
                                               value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_100G_FEC_COL_INS_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_100G_FEC_COL_INS_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000007c bits 9) field OTU_FRM_100G_FEC_COL_INS_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    reg_value = otu_frm_reg_OTU_FRM_100G_ENABLE_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_INS_EN_MSK) >> OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_INS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_100G_FEC_COL_INS_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_100G_DINTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_100G_DINTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_100G_DINTLV_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_100G_DINTLV_EN_set", value );

    /* (0x0000007c bits 8) field OTU_FRM_100G_DINTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( b_ptr,
                                               h_ptr,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DINTLV_EN_MSK,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DINTLV_EN_OFF,
                                               value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_100G_DINTLV_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_100G_DINTLV_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000007c bits 8) field OTU_FRM_100G_DINTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    reg_value = otu_frm_reg_OTU_FRM_100G_ENABLE_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DINTLV_EN_MSK) >> OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DINTLV_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_100G_DINTLV_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_100G_MI_ACTIVE_set( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_100G_MI_ACTIVE_set( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_100G_MI_ACTIVE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_100G_MI_ACTIVE_set", value );

    /* (0x0000007c bits 6) field OTU_FRM_100G_MI_ACTIVE of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( b_ptr,
                                               h_ptr,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_MI_ACTIVE_MSK,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_MI_ACTIVE_OFF,
                                               value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_100G_MI_ACTIVE_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_100G_MI_ACTIVE_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000007c bits 6) field OTU_FRM_100G_MI_ACTIVE of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    reg_value = otu_frm_reg_OTU_FRM_100G_ENABLE_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_MI_ACTIVE_MSK) >> OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_MI_ACTIVE_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_100G_MI_ACTIVE_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_set", value );

    /* (0x0000007c bits 5) field OTU_FRM_100G_DLOM_TO_SF_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( b_ptr,
                                               h_ptr,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_TO_SF_EN_MSK,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_TO_SF_EN_OFF,
                                               value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000007c bits 5) field OTU_FRM_100G_DLOM_TO_SF_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    reg_value = otu_frm_reg_OTU_FRM_100G_ENABLE_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_TO_SF_EN_MSK) >> OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_TO_SF_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_100G_DLOM_EN_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_100G_DLOM_EN_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_100G_DLOM_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_100G_DLOM_EN_set", value );

    /* (0x0000007c bits 4) field OTU_FRM_100G_DLOM_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( b_ptr,
                                               h_ptr,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_EN_MSK,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_EN_OFF,
                                               value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_100G_DLOM_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_100G_DLOM_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000007c bits 4) field OTU_FRM_100G_DLOM_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    reg_value = otu_frm_reg_OTU_FRM_100G_ENABLE_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_EN_MSK) >> OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_100G_DLOM_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_100G_SF_SQUELCH_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_100G_SF_SQUELCH_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_100G_SF_SQUELCH_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_100G_SF_SQUELCH_EN_set", value );

    /* (0x0000007c bits 3) field OTU_FRM_100G_SF_SQUELCH_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( b_ptr,
                                               h_ptr,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SF_SQUELCH_EN_MSK,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SF_SQUELCH_EN_OFF,
                                               value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_100G_SF_SQUELCH_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_100G_SF_SQUELCH_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000007c bits 3) field OTU_FRM_100G_SF_SQUELCH_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    reg_value = otu_frm_reg_OTU_FRM_100G_ENABLE_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SF_SQUELCH_EN_MSK) >> OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SF_SQUELCH_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_100G_SF_SQUELCH_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_set", value );

    /* (0x0000007c bits 2) field OTU_FRM_100G_DESCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( b_ptr,
                                               h_ptr,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DESCRAMBLING_EN_MSK,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DESCRAMBLING_EN_OFF,
                                               value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000007c bits 2) field OTU_FRM_100G_DESCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    reg_value = otu_frm_reg_OTU_FRM_100G_ENABLE_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DESCRAMBLING_EN_MSK) >> OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DESCRAMBLING_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_100G_FEC_COL_RM_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_100G_FEC_COL_RM_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_100G_FEC_COL_RM_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_100G_FEC_COL_RM_EN_set", value );

    /* (0x0000007c bits 1) field OTU_FRM_100G_FEC_COL_RM_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( b_ptr,
                                               h_ptr,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_RM_EN_MSK,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_RM_EN_OFF,
                                               value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_100G_FEC_COL_RM_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_100G_FEC_COL_RM_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000007c bits 1) field OTU_FRM_100G_FEC_COL_RM_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    reg_value = otu_frm_reg_OTU_FRM_100G_ENABLE_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_RM_EN_MSK) >> OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_RM_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_100G_FEC_COL_RM_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_100G_INTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_100G_INTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_100G_INTLV_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_100G_INTLV_EN_set", value );

    /* (0x0000007c bits 0) field OTU_FRM_100G_INTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    otu_frm_reg_OTU_FRM_100G_ENABLE_field_set( b_ptr,
                                               h_ptr,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_INTLV_EN_MSK,
                                               OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_INTLV_EN_OFF,
                                               value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_100G_INTLV_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_100G_INTLV_EN_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000007c bits 0) field OTU_FRM_100G_INTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE */
    reg_value = otu_frm_reg_OTU_FRM_100G_ENABLE_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_INTLV_EN_MSK) >> OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_INTLV_EN_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_100G_INTLV_EN_get", value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_SPARE_set( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_SPARE_set( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_SPARE_set", value );

    /* (0x000000e8 bits 31:0) field OTU_FRM_SPARE of register PMC_OTU_FRM_REG_OTU_FRM_SPARE */
    otu_frm_reg_OTU_FRM_SPARE_field_set( b_ptr,
                                         h_ptr,
                                         OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_MSK,
                                         OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_OFF,
                                         value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_SPARE_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_SPARE_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e8 bits 31:0) field OTU_FRM_SPARE of register PMC_OTU_FRM_REG_OTU_FRM_SPARE */
    reg_value = otu_frm_reg_OTU_FRM_SPARE_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_MSK) >> OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_SPARE_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_SPARE_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_SPARE_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_SPARE_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_SPARE_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_SPARE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000e8 bits 31:0) field OTU_FRM_SPARE of register PMC_OTU_FRM_REG_OTU_FRM_SPARE */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000e8 bits 31:0) field OTU_FRM_SPARE of register PMC_OTU_FRM_REG_OTU_FRM_SPARE */
        otu_frm_reg_OTU_FRM_SPARE_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_OFF + subfield_offset),
                                             OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_SPARE_get( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_SPARE_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_SPARE_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_SPARE_get", stop_bit, 31 );
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
    /* (0x000000e8 bits 31:0) field OTU_FRM_SPARE of register PMC_OTU_FRM_REG_OTU_FRM_SPARE */
    reg_value = otu_frm_reg_OTU_FRM_SPARE_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_MSK)
                  >> OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_MSK, OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_SPARE_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_set", N, value );

    /* ((0x00000040 + (N) * 0x04) bits 10) field OTU_FRM_10G_SCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SCRAMBLING_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SCRAMBLING_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_get", N, 11);
    /* ((0x00000040 + (N) * 0x04) bits 10) field OTU_FRM_10G_SCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SCRAMBLING_EN_MSK) >> OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SCRAMBLING_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_set", N, value );

    /* ((0x00000040 + (N) * 0x04) bits 9) field OTU_FRM_10G_FEC_COL_INS_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_INS_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_INS_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_get( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_get( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_get", N, 11);
    /* ((0x00000040 + (N) * 0x04) bits 9) field OTU_FRM_10G_FEC_COL_INS_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_INS_EN_MSK) >> OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_INS_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_10G_DINTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_10G_DINTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_DINTLV_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_10G_DINTLV_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_DINTLV_EN_set", N, value );

    /* ((0x00000040 + (N) * 0x04) bits 8) field OTU_FRM_10G_DINTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DINTLV_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DINTLV_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_10G_DINTLV_EN_get( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_10G_DINTLV_EN_get( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_DINTLV_EN_get", N, 11);
    /* ((0x00000040 + (N) * 0x04) bits 8) field OTU_FRM_10G_DINTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DINTLV_EN_MSK) >> OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DINTLV_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_DINTLV_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_10G_MI_ACTIVE_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_10G_MI_ACTIVE_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_MI_ACTIVE_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_10G_MI_ACTIVE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_MI_ACTIVE_set", N, value );

    /* ((0x00000040 + (N) * 0x04) bits 6) field OTU_FRM_10G_MI_ACTIVE of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_MI_ACTIVE_MSK,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_MI_ACTIVE_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_10G_MI_ACTIVE_get( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_10G_MI_ACTIVE_get( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_MI_ACTIVE_get", N, 11);
    /* ((0x00000040 + (N) * 0x04) bits 6) field OTU_FRM_10G_MI_ACTIVE of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_MI_ACTIVE_MSK) >> OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_MI_ACTIVE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_MI_ACTIVE_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_set", N, value );

    /* ((0x00000040 + (N) * 0x04) bits 5) field OTU_FRM_10G_DLOM_TO_SF_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_TO_SF_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_TO_SF_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_get", N, 11);
    /* ((0x00000040 + (N) * 0x04) bits 5) field OTU_FRM_10G_DLOM_TO_SF_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_TO_SF_EN_MSK) >> OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_TO_SF_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_10G_DLOM_EN_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_10G_DLOM_EN_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_DLOM_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_10G_DLOM_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_DLOM_EN_set", N, value );

    /* ((0x00000040 + (N) * 0x04) bits 4) field OTU_FRM_10G_DLOM_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_10G_DLOM_EN_get( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_10G_DLOM_EN_get( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_DLOM_EN_get", N, 11);
    /* ((0x00000040 + (N) * 0x04) bits 4) field OTU_FRM_10G_DLOM_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_EN_MSK) >> OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_DLOM_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_set", N, value );

    /* ((0x00000040 + (N) * 0x04) bits 3) field OTU_FRM_10G_SF_SQUELCH_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SF_SQUELCH_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SF_SQUELCH_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_get", N, 11);
    /* ((0x00000040 + (N) * 0x04) bits 3) field OTU_FRM_10G_SF_SQUELCH_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SF_SQUELCH_EN_MSK) >> OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SF_SQUELCH_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_set", N, value );

    /* ((0x00000040 + (N) * 0x04) bits 2) field OTU_FRM_10G_DESCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DESCRAMBLING_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DESCRAMBLING_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_get", N, 11);
    /* ((0x00000040 + (N) * 0x04) bits 2) field OTU_FRM_10G_DESCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DESCRAMBLING_EN_MSK) >> OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DESCRAMBLING_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_set", N, value );

    /* ((0x00000040 + (N) * 0x04) bits 1) field OTU_FRM_10G_FEC_COL_RM_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_RM_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_RM_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_get", N, 11);
    /* ((0x00000040 + (N) * 0x04) bits 1) field OTU_FRM_10G_FEC_COL_RM_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_RM_EN_MSK) >> OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_RM_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_10G_INTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_10G_INTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_INTLV_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_10G_INTLV_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_INTLV_EN_set", N, value );

    /* ((0x00000040 + (N) * 0x04) bits 0) field OTU_FRM_10G_INTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_INTLV_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_INTLV_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_10G_INTLV_EN_get( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_10G_INTLV_EN_get( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_10G_INTLV_EN_get", N, 11);
    /* ((0x00000040 + (N) * 0x04) bits 0) field OTU_FRM_10G_INTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_10G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_INTLV_EN_MSK) >> OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_INTLV_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_10G_INTLV_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_set", N, 11);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_set", value, 511);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_set", N, value );

    /* ((0x00000080 + (N) * 0x04) bits 24:16) bits 0:8 use field OTU_FRM_DINTLV_XOFF_THRES of register PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG index N=0..11 */
    otu_frm_reg_OTU_FRM_STREAM_CFG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_MSK,
                                                    OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_get", N, 11);
    /* ((0x00000080 + (N) * 0x04) bits 24:16) bits 0:8 use field OTU_FRM_DINTLV_XOFF_THRES of register PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_STREAM_CFG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_MSK) >> OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_get", N, value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_set( otu_frm_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_set( otu_frm_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_set", stop_bit, 8 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000080 + (N) * 0x04) bits 24:16) bits 0:8 use field OTU_FRM_DINTLV_XOFF_THRES of register PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG index N=0..11 */
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
        if (stop_bit < 8) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 8;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000080 + (N) * 0x04) bits 24:16) bits 0:8 use field OTU_FRM_DINTLV_XOFF_THRES of register PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG index N=0..11 */
        otu_frm_reg_OTU_FRM_STREAM_CFG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_OFF + subfield_offset),
                                                        OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_get( otu_frm_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_get( otu_frm_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_get", stop_bit, 8 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 8) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 8;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000080 + (N) * 0x04) bits 24:16) bits 0:8 use field OTU_FRM_DINTLV_XOFF_THRES of register PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_STREAM_CFG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_MSK)
                  >> OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_MSK, OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_CH_ID_set( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_CH_ID_set( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_CH_ID_set", N, 11);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_CH_ID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_CH_ID_set", N, value );

    /* ((0x00000080 + (N) * 0x04) bits 6:0) bits 0:6 use field OTU_FRM_CH_ID of register PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG index N=0..11 */
    otu_frm_reg_OTU_FRM_STREAM_CFG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_MSK,
                                                    OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_CH_ID_get( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_CH_ID_get( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_CH_ID_get", N, 11);
    /* ((0x00000080 + (N) * 0x04) bits 6:0) bits 0:6 use field OTU_FRM_CH_ID of register PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_STREAM_CFG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_MSK) >> OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_CH_ID_get", N, value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_CH_ID_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_CH_ID_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_range_OTU_FRM_CH_ID_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_CH_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_CH_ID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_CH_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000080 + (N) * 0x04) bits 6:0) bits 0:6 use field OTU_FRM_CH_ID of register PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG index N=0..11 */
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
        if (stop_bit < 6) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 6;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000080 + (N) * 0x04) bits 6:0) bits 0:6 use field OTU_FRM_CH_ID of register PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG index N=0..11 */
        otu_frm_reg_OTU_FRM_STREAM_CFG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_OFF + subfield_offset),
                                                        OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_CH_ID_get( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_CH_ID_get( otu_frm_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_range_OTU_FRM_CH_ID_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_CH_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_CH_ID_get", stop_bit, 6 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 6) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 6;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000080 + (N) * 0x04) bits 6:0) bits 0:6 use field OTU_FRM_CH_ID of register PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG index N=0..11 */
    reg_value = otu_frm_reg_OTU_FRM_STREAM_CFG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_MSK)
                  >> OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_MSK, OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_CH_ID_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size3
 * ==================================================================================
 */
static INLINE void otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_set", N, value );

    /* ((0x00000070 + (N) * 0x04) bits 10) field OTU_FRM_40G_SCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SCRAMBLING_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SCRAMBLING_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_get", N, 2);
    /* ((0x00000070 + (N) * 0x04) bits 10) field OTU_FRM_40G_SCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    reg_value = otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SCRAMBLING_EN_MSK) >> OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SCRAMBLING_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_set", N, value );

    /* ((0x00000070 + (N) * 0x04) bits 9) field OTU_FRM_40G_FEC_COL_INS_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_INS_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_INS_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_get( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_get( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_get", N, 2);
    /* ((0x00000070 + (N) * 0x04) bits 9) field OTU_FRM_40G_FEC_COL_INS_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    reg_value = otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_INS_EN_MSK) >> OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_INS_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_40G_DINTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_40G_DINTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_DINTLV_EN_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_40G_DINTLV_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_DINTLV_EN_set", N, value );

    /* ((0x00000070 + (N) * 0x04) bits 8) field OTU_FRM_40G_DINTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DINTLV_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DINTLV_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_40G_DINTLV_EN_get( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_40G_DINTLV_EN_get( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_DINTLV_EN_get", N, 2);
    /* ((0x00000070 + (N) * 0x04) bits 8) field OTU_FRM_40G_DINTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    reg_value = otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DINTLV_EN_MSK) >> OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DINTLV_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_DINTLV_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_40G_MI_ACTIVE_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_40G_MI_ACTIVE_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_MI_ACTIVE_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_40G_MI_ACTIVE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_MI_ACTIVE_set", N, value );

    /* ((0x00000070 + (N) * 0x04) bits 6) field OTU_FRM_40G_MI_ACTIVE of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_MI_ACTIVE_MSK,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_MI_ACTIVE_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_40G_MI_ACTIVE_get( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_40G_MI_ACTIVE_get( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_MI_ACTIVE_get", N, 2);
    /* ((0x00000070 + (N) * 0x04) bits 6) field OTU_FRM_40G_MI_ACTIVE of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    reg_value = otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_MI_ACTIVE_MSK) >> OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_MI_ACTIVE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_MI_ACTIVE_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_set", N, value );

    /* ((0x00000070 + (N) * 0x04) bits 5) field OTU_FRM_40G_DLOM_TO_SF_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_TO_SF_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_TO_SF_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_get", N, 2);
    /* ((0x00000070 + (N) * 0x04) bits 5) field OTU_FRM_40G_DLOM_TO_SF_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    reg_value = otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_TO_SF_EN_MSK) >> OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_TO_SF_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_40G_DLOM_EN_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_40G_DLOM_EN_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_DLOM_EN_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_40G_DLOM_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_DLOM_EN_set", N, value );

    /* ((0x00000070 + (N) * 0x04) bits 4) field OTU_FRM_40G_DLOM_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_40G_DLOM_EN_get( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_40G_DLOM_EN_get( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_DLOM_EN_get", N, 2);
    /* ((0x00000070 + (N) * 0x04) bits 4) field OTU_FRM_40G_DLOM_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    reg_value = otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_EN_MSK) >> OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_DLOM_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_set", N, value );

    /* ((0x00000070 + (N) * 0x04) bits 3) field OTU_FRM_40G_SF_SQUELCH_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SF_SQUELCH_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SF_SQUELCH_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_get", N, 2);
    /* ((0x00000070 + (N) * 0x04) bits 3) field OTU_FRM_40G_SF_SQUELCH_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    reg_value = otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SF_SQUELCH_EN_MSK) >> OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SF_SQUELCH_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_set", N, value );

    /* ((0x00000070 + (N) * 0x04) bits 2) field OTU_FRM_40G_DESCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DESCRAMBLING_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DESCRAMBLING_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_get( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_get", N, 2);
    /* ((0x00000070 + (N) * 0x04) bits 2) field OTU_FRM_40G_DESCRAMBLING_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    reg_value = otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DESCRAMBLING_EN_MSK) >> OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DESCRAMBLING_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_set( otu_frm_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_set", N, value );

    /* ((0x00000070 + (N) * 0x04) bits 1) field OTU_FRM_40G_FEC_COL_RM_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_RM_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_RM_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_get", N, 2);
    /* ((0x00000070 + (N) * 0x04) bits 1) field OTU_FRM_40G_FEC_COL_RM_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    reg_value = otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_RM_EN_MSK) >> OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_RM_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_get", N, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_40G_INTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_40G_INTLV_EN_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_INTLV_EN_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_40G_INTLV_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_INTLV_EN_set", N, value );

    /* ((0x00000070 + (N) * 0x04) bits 0) field OTU_FRM_40G_INTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_INTLV_EN_MSK,
                                                    OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_INTLV_EN_OFF,
                                                    value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_40G_INTLV_EN_get( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_40G_INTLV_EN_get( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otu_frm_field_OTU_FRM_40G_INTLV_EN_get", N, 2);
    /* ((0x00000070 + (N) * 0x04) bits 0) field OTU_FRM_40G_INTLV_EN of register PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE index N=0..2 */
    reg_value = otu_frm_reg_OTU_FRM_40G_ENABLE_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_INTLV_EN_MSK) >> OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_INTLV_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otu_frm_field_OTU_FRM_40G_INTLV_EN_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void otu_frm_field_OTU_FRM_PMON_UPDATE_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_PMON_UPDATE_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_PMON_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_PMON_UPDATE_set", value );

    /* (0x00000004 bits 0) field OTU_FRM_PMON_UPDATE of register PMC_OTU_FRM_REG_OTU_FRM_SW_PMON */
    otu_frm_reg_OTU_FRM_SW_PMON_field_set( b_ptr,
                                           h_ptr,
                                           OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_PMON_UPDATE_MSK,
                                           OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_PMON_UPDATE_OFF,
                                           value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_PMON_UPDATE_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_PMON_UPDATE_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field OTU_FRM_PMON_UPDATE of register PMC_OTU_FRM_REG_OTU_FRM_SW_PMON */
    reg_value = otu_frm_reg_OTU_FRM_SW_PMON_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_PMON_UPDATE_MSK) >> OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_PMON_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_PMON_UPDATE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void otu_frm_field_OTU_FRM_INTLV_WOVR_E_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_INTLV_WOVR_E_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_INTLV_WOVR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_INTLV_WOVR_E_set", value );

    /* (0x00000100 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_WOVR_E of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E */
    otu_frm_reg_OTU_FRM_INTLV_WOVR_E_field_set( b_ptr,
                                                h_ptr,
                                                OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_MSK,
                                                OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_OFF,
                                                value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_WOVR_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_WOVR_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_WOVR_E of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_WOVR_E_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_MSK) >> OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_INTLV_WOVR_E_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000100 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_WOVR_E of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E */
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
        /* (0x00000100 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_WOVR_E of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E */
        otu_frm_reg_OTU_FRM_INTLV_WOVR_E_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_OFF + subfield_offset),
                                                    OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_get( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_get", stop_bit, 11 );
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
    /* (0x00000100 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_WOVR_E of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_WOVR_E_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_MSK)
                  >> OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_MSK, OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_INTLV_RUDR_E_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_INTLV_RUDR_E_set( otu_frm_buffer_t *b_ptr,
                                                           otu_frm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_INTLV_RUDR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_INTLV_RUDR_E_set", value );

    /* (0x00000104 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_RUDR_E of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E */
    otu_frm_reg_OTU_FRM_INTLV_RUDR_E_field_set( b_ptr,
                                                h_ptr,
                                                OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_MSK,
                                                OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_OFF,
                                                value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_RUDR_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_RUDR_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_RUDR_E of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_RUDR_E_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_MSK) >> OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_INTLV_RUDR_E_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_set( otu_frm_buffer_t *b_ptr,
                                                                 otu_frm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000104 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_RUDR_E of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E */
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
        /* (0x00000104 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_RUDR_E of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E */
        otu_frm_reg_OTU_FRM_INTLV_RUDR_E_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_OFF + subfield_offset),
                                                    OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_get( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_get", stop_bit, 11 );
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
    /* (0x00000104 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_RUDR_E of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_RUDR_E_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_MSK)
                  >> OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_MSK, OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_DINTLV_WOVR_E_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_DINTLV_WOVR_E_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_WOVR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_WOVR_E_set", value );

    /* (0x00000108 bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_WOVR_E of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E */
    otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_field_set( b_ptr,
                                                 h_ptr,
                                                 OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_MSK,
                                                 OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_OFF,
                                                 value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_WOVR_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_WOVR_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000108 bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_WOVR_E of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_MSK) >> OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_WOVR_E_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000108 bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_WOVR_E of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E */
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
        /* (0x00000108 bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_WOVR_E of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E */
        otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_OFF + subfield_offset),
                                                     OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_get( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_get", stop_bit, 11 );
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
    /* (0x00000108 bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_WOVR_E of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_MSK)
                  >> OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_MSK, OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_DINTLV_RUDR_E_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_DINTLV_RUDR_E_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_RUDR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_RUDR_E_set", value );

    /* (0x0000010c bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_RUDR_E of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E */
    otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_field_set( b_ptr,
                                                 h_ptr,
                                                 OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_MSK,
                                                 OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_OFF,
                                                 value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_RUDR_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_RUDR_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000010c bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_RUDR_E of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_MSK) >> OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_RUDR_E_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_set( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000010c bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_RUDR_E of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E */
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
        /* (0x0000010c bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_RUDR_E of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E */
        otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_OFF + subfield_offset),
                                                     OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_get( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_get", stop_bit, 11 );
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
    /* (0x0000010c bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_RUDR_E of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_MSK)
                  >> OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_MSK, OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_D_LOM_E_set( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_D_LOM_E_set( otu_frm_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_D_LOM_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_D_LOM_E_set", value );

    /* (0x00000110 bits 11:0) bits 0:11 use field OTU_FRM_D_LOM_E of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_E */
    otu_frm_reg_OTU_FRM_D_LOM_E_field_set( b_ptr,
                                           h_ptr,
                                           OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_MSK,
                                           OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_OFF,
                                           value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_D_LOM_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_D_LOM_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000110 bits 11:0) bits 0:11 use field OTU_FRM_D_LOM_E of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_E */
    reg_value = otu_frm_reg_OTU_FRM_D_LOM_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_MSK) >> OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_D_LOM_E_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_D_LOM_E_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_D_LOM_E_set( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_D_LOM_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_D_LOM_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_D_LOM_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000110 bits 11:0) bits 0:11 use field OTU_FRM_D_LOM_E of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_E */
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
        /* (0x00000110 bits 11:0) bits 0:11 use field OTU_FRM_D_LOM_E of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_E */
        otu_frm_reg_OTU_FRM_D_LOM_E_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_OFF + subfield_offset),
                                               OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_D_LOM_E_get( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_D_LOM_E_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_D_LOM_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_D_LOM_E_get", stop_bit, 11 );
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
    /* (0x00000110 bits 11:0) bits 0:11 use field OTU_FRM_D_LOM_E of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_E */
    reg_value = otu_frm_reg_OTU_FRM_D_LOM_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_MSK)
                  >> OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_MSK, OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_D_LOM_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otu_frm_field_OTU_FRM_OOM_E_set( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_OOM_E_set( otu_frm_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_OOM_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_OOM_E_set", value );

    /* (0x00000114 bits 11:0) bits 0:11 use field OTU_FRM_OOM_E of register PMC_OTU_FRM_REG_OTU_FRM_OOM_E */
    otu_frm_reg_OTU_FRM_OOM_E_field_set( b_ptr,
                                         h_ptr,
                                         OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_MSK,
                                         OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_OFF,
                                         value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_OOM_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_OOM_E_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 11:0) bits 0:11 use field OTU_FRM_OOM_E of register PMC_OTU_FRM_REG_OTU_FRM_OOM_E */
    reg_value = otu_frm_reg_OTU_FRM_OOM_E_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_MSK) >> OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_OOM_E_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_OOM_E_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_OOM_E_set( otu_frm_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_OOM_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_OOM_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_OOM_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000114 bits 11:0) bits 0:11 use field OTU_FRM_OOM_E of register PMC_OTU_FRM_REG_OTU_FRM_OOM_E */
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
        /* (0x00000114 bits 11:0) bits 0:11 use field OTU_FRM_OOM_E of register PMC_OTU_FRM_REG_OTU_FRM_OOM_E */
        otu_frm_reg_OTU_FRM_OOM_E_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_OFF + subfield_offset),
                                             OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_OOM_E_get( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_OOM_E_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_OOM_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_OOM_E_get", stop_bit, 11 );
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
    /* (0x00000114 bits 11:0) bits 0:11 use field OTU_FRM_OOM_E of register PMC_OTU_FRM_REG_OTU_FRM_OOM_E */
    reg_value = otu_frm_reg_OTU_FRM_OOM_E_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_MSK)
                  >> OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_MSK, OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_OOM_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void otu_frm_field_OTU_FRM_INTLV_WOVR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_INTLV_WOVR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_INTLV_WOVR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_INTLV_WOVR_I_set_to_clear", value );

    /* (0x00000120 bits 11:0) field OTU_FRM_INTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I */
    otu_frm_reg_OTU_FRM_INTLV_WOVR_I_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_MSK,
                                                                OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_OFF,
                                                                value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_WOVR_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_WOVR_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000120 bits 11:0) field OTU_FRM_INTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_WOVR_I_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_MSK) >> OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_INTLV_WOVR_I_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000120 bits 11:0) field OTU_FRM_INTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I */
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
        /* (0x00000120 bits 11:0) field OTU_FRM_INTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I */
        otu_frm_reg_OTU_FRM_INTLV_WOVR_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    subfield_mask << (OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_OFF + subfield_offset),
                                                                    OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_OFF + subfield_offset,
                                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_get( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_get", stop_bit, 11 );
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
    /* (0x00000120 bits 11:0) field OTU_FRM_INTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_WOVR_I_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_MSK)
                  >> OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_MSK, OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                                  otu_frm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                                  otu_frm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000120 bits 11:0) field OTU_FRM_INTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I */
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
        /* (0x00000120 bits 11:0) field OTU_FRM_INTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I */
        return otu_frm_reg_OTU_FRM_INTLV_WOVR_I_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_OFF + subfield_offset),
                                                      value << (OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_INTLV_WOVR_I_poll", value );

    /* (0x00000120 bits 11:0) field OTU_FRM_INTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I */
    return otu_frm_reg_OTU_FRM_INTLV_WOVR_I_poll( b_ptr,
                                                  h_ptr,
                                                  OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_MSK,
                                                  (value<<OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_field_OTU_FRM_INTLV_RUDR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_INTLV_RUDR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_INTLV_RUDR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_INTLV_RUDR_I_set_to_clear", value );

    /* (0x00000124 bits 11:0) field OTU_FRM_INTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I */
    otu_frm_reg_OTU_FRM_INTLV_RUDR_I_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_MSK,
                                                                OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_OFF,
                                                                value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_RUDR_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_RUDR_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 11:0) field OTU_FRM_INTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_RUDR_I_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_MSK) >> OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_INTLV_RUDR_I_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                          otu_frm_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000124 bits 11:0) field OTU_FRM_INTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I */
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
        /* (0x00000124 bits 11:0) field OTU_FRM_INTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I */
        otu_frm_reg_OTU_FRM_INTLV_RUDR_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    subfield_mask << (OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_OFF + subfield_offset),
                                                                    OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_OFF + subfield_offset,
                                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_get( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_get", stop_bit, 11 );
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
    /* (0x00000124 bits 11:0) field OTU_FRM_INTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_RUDR_I_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_MSK)
                  >> OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_MSK, OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                                  otu_frm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                                  otu_frm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000124 bits 11:0) field OTU_FRM_INTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I */
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
        /* (0x00000124 bits 11:0) field OTU_FRM_INTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I */
        return otu_frm_reg_OTU_FRM_INTLV_RUDR_I_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_OFF + subfield_offset),
                                                      value << (OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_INTLV_RUDR_I_poll", value );

    /* (0x00000124 bits 11:0) field OTU_FRM_INTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I */
    return otu_frm_reg_OTU_FRM_INTLV_RUDR_I_poll( b_ptr,
                                                  h_ptr,
                                                  OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_MSK,
                                                  (value<<OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_field_OTU_FRM_DINTLV_WOVR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_DINTLV_WOVR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_WOVR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_WOVR_I_set_to_clear", value );

    /* (0x00000128 bits 11:0) field OTU_FRM_DINTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I */
    otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_MSK,
                                                                 OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_OFF,
                                                                 value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_WOVR_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_WOVR_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000128 bits 11:0) field OTU_FRM_DINTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_MSK) >> OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_WOVR_I_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000128 bits 11:0) field OTU_FRM_DINTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I */
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
        /* (0x00000128 bits 11:0) field OTU_FRM_DINTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I */
        otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     subfield_mask << (OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_OFF + subfield_offset),
                                                                     OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_get( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_get", stop_bit, 11 );
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
    /* (0x00000128 bits 11:0) field OTU_FRM_DINTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_MSK)
                  >> OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_MSK, OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                                   otu_frm_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                                   otu_frm_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000128 bits 11:0) field OTU_FRM_DINTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I */
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
        /* (0x00000128 bits 11:0) field OTU_FRM_DINTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I */
        return otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_poll( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_OFF + subfield_offset),
                                                       value << (OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_OFF + subfield_offset),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_WOVR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_DINTLV_WOVR_I_poll", value );

    /* (0x00000128 bits 11:0) field OTU_FRM_DINTLV_WOVR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I */
    return otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_poll( b_ptr,
                                                   h_ptr,
                                                   OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_MSK,
                                                   (value<<OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_field_OTU_FRM_DINTLV_RUDR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_DINTLV_RUDR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_RUDR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_RUDR_I_set_to_clear", value );

    /* (0x0000012c bits 11:0) field OTU_FRM_DINTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I */
    otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_MSK,
                                                                 OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_OFF,
                                                                 value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_RUDR_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_RUDR_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000012c bits 11:0) field OTU_FRM_DINTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_MSK) >> OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_RUDR_I_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000012c bits 11:0) field OTU_FRM_DINTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I */
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
        /* (0x0000012c bits 11:0) field OTU_FRM_DINTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I */
        otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     subfield_mask << (OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_OFF + subfield_offset),
                                                                     OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_get( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_get", stop_bit, 11 );
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
    /* (0x0000012c bits 11:0) field OTU_FRM_DINTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_MSK)
                  >> OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_MSK, OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                                   otu_frm_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                                   otu_frm_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000012c bits 11:0) field OTU_FRM_DINTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I */
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
        /* (0x0000012c bits 11:0) field OTU_FRM_DINTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I */
        return otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_poll( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_OFF + subfield_offset),
                                                       value << (OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_OFF + subfield_offset),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_RUDR_I_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_DINTLV_RUDR_I_poll", value );

    /* (0x0000012c bits 11:0) field OTU_FRM_DINTLV_RUDR_I of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I */
    return otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_poll( b_ptr,
                                                   h_ptr,
                                                   OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_MSK,
                                                   (value<<OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_field_OTU_FRM_D_LOM_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_D_LOM_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                               otu_frm_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_D_LOM_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_D_LOM_I_set_to_clear", value );

    /* (0x00000130 bits 11:0) field OTU_FRM_D_LOM_I of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I */
    otu_frm_reg_OTU_FRM_D_LOM_I_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_MSK,
                                                           OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_OFF,
                                                           value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_D_LOM_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_D_LOM_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000130 bits 11:0) field OTU_FRM_D_LOM_I of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I */
    reg_value = otu_frm_reg_OTU_FRM_D_LOM_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_MSK) >> OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_D_LOM_I_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_D_LOM_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_D_LOM_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_D_LOM_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_D_LOM_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_D_LOM_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000130 bits 11:0) field OTU_FRM_D_LOM_I of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I */
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
        /* (0x00000130 bits 11:0) field OTU_FRM_D_LOM_I of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I */
        otu_frm_reg_OTU_FRM_D_LOM_I_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_OFF + subfield_offset),
                                                               OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_D_LOM_I_get( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_D_LOM_I_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_D_LOM_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_D_LOM_I_get", stop_bit, 11 );
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
    /* (0x00000130 bits 11:0) field OTU_FRM_D_LOM_I of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I */
    reg_value = otu_frm_reg_OTU_FRM_D_LOM_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_MSK)
                  >> OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_MSK, OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_D_LOM_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_D_LOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_D_LOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_D_LOM_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_D_LOM_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_D_LOM_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000130 bits 11:0) field OTU_FRM_D_LOM_I of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I */
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
        /* (0x00000130 bits 11:0) field OTU_FRM_D_LOM_I of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I */
        return otu_frm_reg_OTU_FRM_D_LOM_I_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_OFF + subfield_offset),
                                                 value << (OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_D_LOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_D_LOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_D_LOM_I_poll", value );

    /* (0x00000130 bits 11:0) field OTU_FRM_D_LOM_I of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I */
    return otu_frm_reg_OTU_FRM_D_LOM_I_poll( b_ptr,
                                             h_ptr,
                                             OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_MSK,
                                             (value<<OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void otu_frm_field_OTU_FRM_OOM_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_OTU_FRM_OOM_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otu_frm_field_OTU_FRM_OOM_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otu_frm_field_OTU_FRM_OOM_I_set_to_clear", value );

    /* (0x00000134 bits 11:0) field OTU_FRM_OOM_I of register PMC_OTU_FRM_REG_OTU_FRM_OOM_I */
    otu_frm_reg_OTU_FRM_OOM_I_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_MSK,
                                                         OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_OFF,
                                                         value);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_OOM_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_OOM_I_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000134 bits 11:0) field OTU_FRM_OOM_I of register PMC_OTU_FRM_REG_OTU_FRM_OOM_I */
    reg_value = otu_frm_reg_OTU_FRM_OOM_I_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_MSK) >> OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_OOM_I_get", value );

    return value;
}
static INLINE void otu_frm_field_range_OTU_FRM_OOM_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otu_frm_field_range_OTU_FRM_OOM_I_set_to_clear( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_OOM_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_OOM_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_OOM_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000134 bits 11:0) field OTU_FRM_OOM_I of register PMC_OTU_FRM_REG_OTU_FRM_OOM_I */
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
        /* (0x00000134 bits 11:0) field OTU_FRM_OOM_I of register PMC_OTU_FRM_REG_OTU_FRM_OOM_I */
        otu_frm_reg_OTU_FRM_OOM_I_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_OFF + subfield_offset),
                                                             OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 otu_frm_field_range_OTU_FRM_OOM_I_get( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_OOM_I_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_OOM_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_OOM_I_get", stop_bit, 11 );
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
    /* (0x00000134 bits 11:0) field OTU_FRM_OOM_I of register PMC_OTU_FRM_REG_OTU_FRM_OOM_I */
    reg_value = otu_frm_reg_OTU_FRM_OOM_I_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_MSK)
                  >> OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_MSK, OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_OOM_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_OOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_OOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_OOM_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_OOM_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_OOM_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000134 bits 11:0) field OTU_FRM_OOM_I of register PMC_OTU_FRM_REG_OTU_FRM_OOM_I */
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
        /* (0x00000134 bits 11:0) field OTU_FRM_OOM_I of register PMC_OTU_FRM_REG_OTU_FRM_OOM_I */
        return otu_frm_reg_OTU_FRM_OOM_I_poll( b_ptr,
                                               h_ptr,
                                               subfield_mask << (OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_OFF + subfield_offset),
                                               value << (OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_OFF + subfield_offset),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_OOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_OOM_I_poll( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_OOM_I_poll", value );

    /* (0x00000134 bits 11:0) field OTU_FRM_OOM_I of register PMC_OTU_FRM_REG_OTU_FRM_OOM_I */
    return otu_frm_reg_OTU_FRM_OOM_I_poll( b_ptr,
                                           h_ptr,
                                           OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_MSK,
                                           (value<<OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_OFF),
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
static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_WOVR_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_WOVR_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000160 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_WOVR_V of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_WOVR_V_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_MSK) >> OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_INTLV_WOVR_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_get( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_get", stop_bit, 11 );
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
    /* (0x00000160 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_WOVR_V of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_WOVR_V_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_MSK)
                  >> OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_MSK, OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                                  otu_frm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                                  otu_frm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000160 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_WOVR_V of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V */
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
        /* (0x00000160 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_WOVR_V of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V */
        return otu_frm_reg_OTU_FRM_INTLV_WOVR_V_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_OFF + subfield_offset),
                                                      value << (OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_INTLV_WOVR_V_poll", value );

    /* (0x00000160 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_WOVR_V of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V */
    return otu_frm_reg_OTU_FRM_INTLV_WOVR_V_poll( b_ptr,
                                                  h_ptr,
                                                  OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_MSK,
                                                  (value<<OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_RUDR_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_INTLV_RUDR_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000164 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_RUDR_V of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_RUDR_V_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_MSK) >> OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_INTLV_RUDR_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_get( otu_frm_buffer_t *b_ptr,
                                                                   otu_frm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_get", stop_bit, 11 );
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
    /* (0x00000164 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_RUDR_V of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V */
    reg_value = otu_frm_reg_OTU_FRM_INTLV_RUDR_V_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_MSK)
                  >> OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_MSK, OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                                  otu_frm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                                  otu_frm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000164 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_RUDR_V of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V */
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
        /* (0x00000164 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_RUDR_V of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V */
        return otu_frm_reg_OTU_FRM_INTLV_RUDR_V_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_OFF + subfield_offset),
                                                      value << (OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                            otu_frm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_INTLV_RUDR_V_poll", value );

    /* (0x00000164 bits 11:0) bits 0:11 use field OTU_FRM_INTLV_RUDR_V of register PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V */
    return otu_frm_reg_OTU_FRM_INTLV_RUDR_V_poll( b_ptr,
                                                  h_ptr,
                                                  OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_MSK,
                                                  (value<<OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_WOVR_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_WOVR_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_WOVR_V of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_MSK) >> OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_WOVR_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_get( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_get", stop_bit, 11 );
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
    /* (0x00000168 bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_WOVR_V of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_MSK)
                  >> OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_MSK, OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                                   otu_frm_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                                   otu_frm_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000168 bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_WOVR_V of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V */
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
        /* (0x00000168 bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_WOVR_V of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V */
        return otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_poll( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_OFF + subfield_offset),
                                                       value << (OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_OFF + subfield_offset),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_WOVR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_DINTLV_WOVR_V_poll", value );

    /* (0x00000168 bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_WOVR_V of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V */
    return otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_poll( b_ptr,
                                                   h_ptr,
                                                   OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_MSK,
                                                   (value<<OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_RUDR_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_DINTLV_RUDR_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000016c bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_RUDR_V of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_MSK) >> OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_DINTLV_RUDR_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_get( otu_frm_buffer_t *b_ptr,
                                                                    otu_frm_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_get", stop_bit, 11 );
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
    /* (0x0000016c bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_RUDR_V of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V */
    reg_value = otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_MSK)
                  >> OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_MSK, OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                                   otu_frm_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                                   otu_frm_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000016c bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_RUDR_V of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V */
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
        /* (0x0000016c bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_RUDR_V of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V */
        return otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_poll( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_OFF + subfield_offset),
                                                       value << (OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_OFF + subfield_offset),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_RUDR_V_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_DINTLV_RUDR_V_poll", value );

    /* (0x0000016c bits 11:0) bits 0:11 use field OTU_FRM_DINTLV_RUDR_V of register PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V */
    return otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_poll( b_ptr,
                                                   h_ptr,
                                                   OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_MSK,
                                                   (value<<OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_OFF),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_D_LOM_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_D_LOM_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000170 bits 11:0) bits 0:11 use field OTU_FRM_D_LOM_V of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_V */
    reg_value = otu_frm_reg_OTU_FRM_D_LOM_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_MSK) >> OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_D_LOM_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_field_range_OTU_FRM_D_LOM_V_get( otu_frm_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_D_LOM_V_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_D_LOM_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_D_LOM_V_get", stop_bit, 11 );
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
    /* (0x00000170 bits 11:0) bits 0:11 use field OTU_FRM_D_LOM_V of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_V */
    reg_value = otu_frm_reg_OTU_FRM_D_LOM_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_MSK)
                  >> OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_MSK, OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_D_LOM_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_D_LOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_D_LOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                             otu_frm_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_D_LOM_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_D_LOM_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_D_LOM_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000170 bits 11:0) bits 0:11 use field OTU_FRM_D_LOM_V of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_V */
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
        /* (0x00000170 bits 11:0) bits 0:11 use field OTU_FRM_D_LOM_V of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_V */
        return otu_frm_reg_OTU_FRM_D_LOM_V_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_OFF + subfield_offset),
                                                 value << (OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_D_LOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_D_LOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                       otu_frm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_D_LOM_V_poll", value );

    /* (0x00000170 bits 11:0) bits 0:11 use field OTU_FRM_D_LOM_V of register PMC_OTU_FRM_REG_OTU_FRM_D_LOM_V */
    return otu_frm_reg_OTU_FRM_D_LOM_V_poll( b_ptr,
                                             h_ptr,
                                             OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_MSK,
                                             (value<<OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE UINT32 otu_frm_field_OTU_FRM_OOM_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_OOM_V_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000174 bits 11:0) bits 0:11 use field OTU_FRM_OOM_V of register PMC_OTU_FRM_REG_OTU_FRM_OOM_V */
    reg_value = otu_frm_reg_OTU_FRM_OOM_V_read(  b_ptr, h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_MSK) >> OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_OOM_V_get", value );

    return value;
}
static INLINE UINT32 otu_frm_field_range_OTU_FRM_OOM_V_get( otu_frm_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_OOM_V_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_OOM_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_OOM_V_get", stop_bit, 11 );
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
    /* (0x00000174 bits 11:0) bits 0:11 use field OTU_FRM_OOM_V of register PMC_OTU_FRM_REG_OTU_FRM_OOM_V */
    reg_value = otu_frm_reg_OTU_FRM_OOM_V_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_MSK)
                  >> OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_MSK, OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_OOM_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_OOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_OOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_OOM_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_OOM_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_OOM_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000174 bits 11:0) bits 0:11 use field OTU_FRM_OOM_V of register PMC_OTU_FRM_REG_OTU_FRM_OOM_V */
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
        /* (0x00000174 bits 11:0) bits 0:11 use field OTU_FRM_OOM_V of register PMC_OTU_FRM_REG_OTU_FRM_OOM_V */
        return otu_frm_reg_OTU_FRM_OOM_V_poll( b_ptr,
                                               h_ptr,
                                               subfield_mask << (OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_OFF + subfield_offset),
                                               value << (OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_OFF + subfield_offset),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_OOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_OOM_V_poll( otu_frm_buffer_t *b_ptr,
                                                                     otu_frm_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otu_frm_field_OTU_FRM_OOM_V_poll", value );

    /* (0x00000174 bits 11:0) bits 0:11 use field OTU_FRM_OOM_V of register PMC_OTU_FRM_REG_OTU_FRM_OOM_V */
    return otu_frm_reg_OTU_FRM_OOM_V_poll( b_ptr,
                                           h_ptr,
                                           OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_MSK,
                                           (value<<OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_OFF),
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
static INLINE UINT32 otu_frm_field_OTU_FRM_INT_SUMMARY_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_OTU_FRM_INT_SUMMARY_get( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000fc bits 29:0) bits 0:29 use field OTU_FRM_INT_SUMMARY of register PMC_OTU_FRM_REG_OTU_FRM_INT_SUMMARY */
    reg_value = otu_frm_reg_OTU_FRM_INT_SUMMARY_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTU_FRM_REG_OTU_FRM_INT_SUMMARY_BIT_OTU_FRM_INT_SUMMARY_MSK) >> OTU_FRM_REG_OTU_FRM_INT_SUMMARY_BIT_OTU_FRM_INT_SUMMARY_OFF;
    IOLOG( "%s -> 0x%08x", "otu_frm_field_OTU_FRM_INT_SUMMARY_get", value );

    return value;
}
static INLINE UINT32 otu_frm_field_range_OTU_FRM_INT_SUMMARY_get( otu_frm_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otu_frm_field_range_OTU_FRM_INT_SUMMARY_get( otu_frm_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otu_frm_field_range_OTU_FRM_INT_SUMMARY_get", stop_bit, start_bit );
    if (stop_bit > 29) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otu_frm_field_range_OTU_FRM_INT_SUMMARY_get", stop_bit, 29 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 29) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 29;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000000fc bits 29:0) bits 0:29 use field OTU_FRM_INT_SUMMARY of register PMC_OTU_FRM_REG_OTU_FRM_INT_SUMMARY */
    reg_value = otu_frm_reg_OTU_FRM_INT_SUMMARY_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTU_FRM_REG_OTU_FRM_INT_SUMMARY_BIT_OTU_FRM_INT_SUMMARY_MSK)
                  >> OTU_FRM_REG_OTU_FRM_INT_SUMMARY_BIT_OTU_FRM_INT_SUMMARY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTU_FRM_REG_OTU_FRM_INT_SUMMARY_BIT_OTU_FRM_INT_SUMMARY_MSK, OTU_FRM_REG_OTU_FRM_INT_SUMMARY_BIT_OTU_FRM_INT_SUMMARY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otu_frm_field_range_OTU_FRM_INT_SUMMARY_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OTU_FRM_IO_INLINE_H */
