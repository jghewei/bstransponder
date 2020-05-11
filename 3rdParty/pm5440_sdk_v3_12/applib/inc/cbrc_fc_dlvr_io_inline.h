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
 *     and register accessor functions for the cbrc_fc_dlvr block
 *****************************************************************************/
#ifndef _CBRC_FC_DLVR_IO_INLINE_H
#define _CBRC_FC_DLVR_IO_INLINE_H

#include "cbrc_api.h"
#include "cbrc_fc_dlvr_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CBRC_FC_DLVR_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for cbrc_fc_dlvr
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
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} cbrc_fc_dlvr_buffer_t;
static INLINE void cbrc_fc_dlvr_buffer_init( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_buffer_init( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "cbrc_fc_dlvr_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void cbrc_fc_dlvr_buffer_flush( cbrc_fc_dlvr_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_buffer_flush( cbrc_fc_dlvr_buffer_t *b_ptr )
{
    IOLOG( "cbrc_fc_dlvr_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 cbrc_fc_dlvr_reg_read( cbrc_fc_dlvr_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_reg_read( cbrc_fc_dlvr_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
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
static INLINE void cbrc_fc_dlvr_reg_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
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

static INLINE void cbrc_fc_dlvr_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 mask,
                                           UINT32 unused_mask,
                                           UINT32 ofs,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
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

static INLINE void cbrc_fc_dlvr_action_on_write_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 mem_type,
                                                           UINT32 reg,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_action_on_write_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
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

static INLINE void cbrc_fc_dlvr_burst_read( cbrc_fc_dlvr_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_burst_read( cbrc_fc_dlvr_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
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

static INLINE void cbrc_fc_dlvr_burst_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_burst_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE cbrc_fc_dlvr_poll( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 value,
                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                      UINT32 max_count,
                                                      UINT32 *num_failed_polls,
                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbrc_fc_dlvr_poll( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
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
 *  register access functions for cbrc_fc_dlvr
 * ==================================================================================
 */

static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_write", value );
    cbrc_fc_dlvr_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0,
                            value);
}

static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_field_set", mask, ofs, value );
    cbrc_fc_dlvr_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0,
                            mask,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_read( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_read( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cbrc_fc_dlvr_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0);

    IOLOG( "%s -> 0x%08x;", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_read", reg_value);
    return reg_value;
}

static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_write", value );
    cbrc_fc_dlvr_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1,
                            value);
}

static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_field_set", mask, ofs, value );
    cbrc_fc_dlvr_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1,
                            mask,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_read( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_read( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cbrc_fc_dlvr_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_write", value );
    cbrc_fc_dlvr_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2,
                            value);
}

static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_field_set", mask, ofs, value );
    cbrc_fc_dlvr_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2,
                            mask,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_read( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_read( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cbrc_fc_dlvr_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_write", value );
    cbrc_fc_dlvr_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3,
                            value);
}

static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_field_set", mask, ofs, value );
    cbrc_fc_dlvr_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3,
                            mask,
                            PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_read( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_read( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cbrc_fc_dlvr_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3);

    IOLOG( "%s -> 0x%08x;", "cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_read", reg_value);
    return reg_value;
}

static INLINE void cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_write", value );
    cbrc_fc_dlvr_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG,
                            value);
}

static INLINE void cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_field_set", mask, ofs, value );
    cbrc_fc_dlvr_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG,
                            mask,
                            PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_read( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_read( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cbrc_fc_dlvr_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG);

    IOLOG( "%s -> 0x%08x;", "cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_read", reg_value);
    return reg_value;
}

static INLINE void cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_write( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_write", value );
    cbrc_fc_dlvr_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG(N),
                            value);
}

static INLINE void cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                          cbrc_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_field_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                          cbrc_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_field_set", N, mask, ofs, value );
    cbrc_fc_dlvr_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG(N),
                            mask,
                            PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_read( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_read( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = cbrc_fc_dlvr_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN0_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN0_set", value );

    /* (0x00053000 bits 27:16) field FC1200_PTD_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053000 bits 27:16) field FC1200_PTD_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN0_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00053000 bits 27:16) field FC1200_PTD_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
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
        /* (0x00053000 bits 27:16) field FC1200_PTD_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
        cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_OFF + subfield_offset),
                                                          CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_get", stop_bit, 11 );
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
    /* (0x00053000 bits 27:16) field FC1200_PTD_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_MSK)
                  >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_MSK, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN0_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN0_set", value );

    /* (0x00053000 bits 15:4) field FC800_PMG_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053000 bits 15:4) field FC800_PMG_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN0_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00053000 bits 15:4) field FC800_PMG_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
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
        /* (0x00053000 bits 15:4) field FC800_PMG_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
        cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_OFF + subfield_offset),
                                                          CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_get", stop_bit, 11 );
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
    /* (0x00053000 bits 15:4) field FC800_PMG_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_MSK)
                  >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_MSK, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_PKTDINT_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_PKTDINT_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN0_set", value );

    /* (0x00053000 bits 2) field PKTDINT_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_PKTDINT_IRQ_EN0_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_PKTDINT_IRQ_EN0_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053000 bits 2) field PKTDINT_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_PKTDINT_IRQ_EN0_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_PKTDINT_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN0_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_MPMO_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_MPMO_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN0_set", value );

    /* (0x00053000 bits 1) field MPMO_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_MPMO_IRQ_EN0_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_MPMO_IRQ_EN0_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053000 bits 1) field MPMO_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_MPMO_IRQ_EN0_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_MPMO_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN0_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_CBRDINT_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_CBRDINT_IRQ_EN0_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN0_set", value );

    /* (0x00053000 bits 0) field CBRDINT_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_CBRDINT_IRQ_EN0_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_CBRDINT_IRQ_EN0_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN0_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053000 bits 0) field CBRDINT_IRQ_EN0 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_0_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_CBRDINT_IRQ_EN0_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_CBRDINT_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN0_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN1_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN1_set", value );

    /* (0x00053004 bits 27:16) field FC1200_PTD_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053004 bits 27:16) field FC1200_PTD_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN1_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00053004 bits 27:16) field FC1200_PTD_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
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
        /* (0x00053004 bits 27:16) field FC1200_PTD_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
        cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_OFF + subfield_offset),
                                                          CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_get", stop_bit, 11 );
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
    /* (0x00053004 bits 27:16) field FC1200_PTD_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_MSK)
                  >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_MSK, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN1_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN1_set", value );

    /* (0x00053004 bits 15:4) field FC800_PMG_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053004 bits 15:4) field FC800_PMG_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN1_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00053004 bits 15:4) field FC800_PMG_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
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
        /* (0x00053004 bits 15:4) field FC800_PMG_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
        cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_OFF + subfield_offset),
                                                          CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_get", stop_bit, 11 );
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
    /* (0x00053004 bits 15:4) field FC800_PMG_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_MSK)
                  >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_MSK, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_PKTDINT_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_PKTDINT_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN1_set", value );

    /* (0x00053004 bits 2) field PKTDINT_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_PKTDINT_IRQ_EN1_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_PKTDINT_IRQ_EN1_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053004 bits 2) field PKTDINT_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_PKTDINT_IRQ_EN1_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_PKTDINT_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN1_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_MPMO_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_MPMO_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN1_set", value );

    /* (0x00053004 bits 1) field MPMO_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_MPMO_IRQ_EN1_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_MPMO_IRQ_EN1_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053004 bits 1) field MPMO_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_MPMO_IRQ_EN1_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_MPMO_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN1_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_CBRDINT_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_CBRDINT_IRQ_EN1_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN1_set", value );

    /* (0x00053004 bits 0) field CBRDINT_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_CBRDINT_IRQ_EN1_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_CBRDINT_IRQ_EN1_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN1_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053004 bits 0) field CBRDINT_IRQ_EN1 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_1_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_CBRDINT_IRQ_EN1_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_CBRDINT_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN1_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN2_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN2_set", value );

    /* (0x00053008 bits 27:16) field FC1200_PTD_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053008 bits 27:16) field FC1200_PTD_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN2_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00053008 bits 27:16) field FC1200_PTD_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
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
        /* (0x00053008 bits 27:16) field FC1200_PTD_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
        cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_OFF + subfield_offset),
                                                          CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_get", stop_bit, 11 );
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
    /* (0x00053008 bits 27:16) field FC1200_PTD_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_MSK)
                  >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_MSK, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN2_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN2_set", value );

    /* (0x00053008 bits 15:4) field FC800_PMG_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053008 bits 15:4) field FC800_PMG_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN2_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00053008 bits 15:4) field FC800_PMG_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
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
        /* (0x00053008 bits 15:4) field FC800_PMG_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
        cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_OFF + subfield_offset),
                                                          CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_get", stop_bit, 11 );
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
    /* (0x00053008 bits 15:4) field FC800_PMG_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_MSK)
                  >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_MSK, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_PKTDINT_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_PKTDINT_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN2_set", value );

    /* (0x00053008 bits 2) field PKTDINT_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_PKTDINT_IRQ_EN2_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_PKTDINT_IRQ_EN2_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053008 bits 2) field PKTDINT_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_PKTDINT_IRQ_EN2_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_PKTDINT_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN2_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_MPMO_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_MPMO_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN2_set", value );

    /* (0x00053008 bits 1) field MPMO_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_MPMO_IRQ_EN2_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_MPMO_IRQ_EN2_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053008 bits 1) field MPMO_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_MPMO_IRQ_EN2_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_MPMO_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN2_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_CBRDINT_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_CBRDINT_IRQ_EN2_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN2_set", value );

    /* (0x00053008 bits 0) field CBRDINT_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_CBRDINT_IRQ_EN2_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_CBRDINT_IRQ_EN2_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN2_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053008 bits 0) field CBRDINT_IRQ_EN2 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_2_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_CBRDINT_IRQ_EN2_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_CBRDINT_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN2_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN3_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN3_set", value );

    /* (0x0005300c bits 27:16) field FC1200_PTD_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0005300c bits 27:16) field FC1200_PTD_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_FC1200_PTD_IRQ_EN3_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0005300c bits 27:16) field FC1200_PTD_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
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
        /* (0x0005300c bits 27:16) field FC1200_PTD_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
        cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_OFF + subfield_offset),
                                                          CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_get", stop_bit, 11 );
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
    /* (0x0005300c bits 27:16) field FC1200_PTD_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_MSK)
                  >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_MSK, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC1200_PTD_IRQ_EN3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN3_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN3_set", value );

    /* (0x0005300c bits 15:4) field FC800_PMG_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0005300c bits 15:4) field FC800_PMG_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_FC800_PMG_IRQ_EN3_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0005300c bits 15:4) field FC800_PMG_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
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
        /* (0x0005300c bits 15:4) field FC800_PMG_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
        cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_OFF + subfield_offset),
                                                          CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_get", stop_bit, 11 );
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
    /* (0x0005300c bits 15:4) field FC800_PMG_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_MSK)
                  >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_MSK, CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cbrc_fc_dlvr_field_range_FC800_PMG_IRQ_EN3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_PKTDINT_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_PKTDINT_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN3_set", value );

    /* (0x0005300c bits 2) field PKTDINT_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_PKTDINT_IRQ_EN3_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_PKTDINT_IRQ_EN3_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_PKTDINT_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0005300c bits 2) field PKTDINT_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_PKTDINT_IRQ_EN3_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_PKTDINT_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_IRQ_EN3_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_MPMO_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_MPMO_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN3_set", value );

    /* (0x0005300c bits 1) field MPMO_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_MPMO_IRQ_EN3_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_MPMO_IRQ_EN3_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_MPMO_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0005300c bits 1) field MPMO_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_MPMO_IRQ_EN3_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_MPMO_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_MPMO_IRQ_EN3_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_CBRDINT_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_CBRDINT_IRQ_EN3_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN3_set", value );

    /* (0x0005300c bits 0) field CBRDINT_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_field_set( b_ptr,
                                                      h_ptr,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_CBRDINT_IRQ_EN3_MSK,
                                                      CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_CBRDINT_IRQ_EN3_OFF,
                                                      value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_CBRDINT_IRQ_EN3_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0005300c bits 0) field CBRDINT_IRQ_EN3 of register PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_TSB_INT_EN_REG_3_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_CBRDINT_IRQ_EN3_MSK) >> CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_CBRDINT_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_IRQ_EN3_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_CBRDINT_RAM_SD_REG_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_CBRDINT_RAM_SD_REG_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_RAM_SD_REG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_RAM_SD_REG_set", value );

    /* (0x00053040 bits 4) field CBRDINT_RAM_SD_REG of register PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG */
    cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBRDINT_RAM_SD_REG_MSK,
                                                     CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBRDINT_RAM_SD_REG_OFF,
                                                     value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_CBRDINT_RAM_SD_REG_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_CBRDINT_RAM_SD_REG_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053040 bits 4) field CBRDINT_RAM_SD_REG of register PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG */
    reg_value = cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBRDINT_RAM_SD_REG_MSK) >> CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBRDINT_RAM_SD_REG_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_CBRDINT_RAM_SD_REG_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_PKTDINT_RAM_SD_REG_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_PKTDINT_RAM_SD_REG_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_RAM_SD_REG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_RAM_SD_REG_set", value );

    /* (0x00053040 bits 3) field PKTDINT_RAM_SD_REG of register PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG */
    cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_PKTDINT_RAM_SD_REG_MSK,
                                                     CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_PKTDINT_RAM_SD_REG_OFF,
                                                     value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_PKTDINT_RAM_SD_REG_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_PKTDINT_RAM_SD_REG_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053040 bits 3) field PKTDINT_RAM_SD_REG of register PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG */
    reg_value = cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_PKTDINT_RAM_SD_REG_MSK) >> CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_PKTDINT_RAM_SD_REG_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_PKTDINT_RAM_SD_REG_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_MPMO_RAM_SD_REG_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_MPMO_RAM_SD_REG_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_MPMO_RAM_SD_REG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_MPMO_RAM_SD_REG_set", value );

    /* (0x00053040 bits 2) field MPMO_RAM_SD_REG of register PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG */
    cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_MPMO_RAM_SD_REG_MSK,
                                                     CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_MPMO_RAM_SD_REG_OFF,
                                                     value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_MPMO_RAM_SD_REG_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_MPMO_RAM_SD_REG_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053040 bits 2) field MPMO_RAM_SD_REG of register PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG */
    reg_value = cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_MPMO_RAM_SD_REG_MSK) >> CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_MPMO_RAM_SD_REG_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_MPMO_RAM_SD_REG_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_CBR_CH_EN_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_CBR_CH_EN_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_CBR_CH_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_CBR_CH_EN_set", value );

    /* (0x00053040 bits 1) field CBR_CH_EN of register PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG */
    cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBR_CH_EN_MSK,
                                                     CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBR_CH_EN_OFF,
                                                     value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_CBR_CH_EN_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_CBR_CH_EN_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053040 bits 1) field CBR_CH_EN of register PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG */
    reg_value = cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBR_CH_EN_MSK) >> CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBR_CH_EN_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_CBR_CH_EN_get", value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_DLVR_SYNC_RST_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_DLVR_SYNC_RST_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_DLVR_SYNC_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbrc_fc_dlvr_field_DLVR_SYNC_RST_set", value );

    /* (0x00053040 bits 0) field DLVR_SYNC_RST of register PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG */
    cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_DLVR_SYNC_RST_MSK,
                                                     CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_DLVR_SYNC_RST_OFF,
                                                     value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_DLVR_SYNC_RST_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_DLVR_SYNC_RST_get( cbrc_fc_dlvr_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00053040 bits 0) field DLVR_SYNC_RST of register PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG */
    reg_value = cbrc_fc_dlvr_reg_DLVR_GLOBAL_CTRL_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_DLVR_SYNC_RST_MSK) >> CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_DLVR_SYNC_RST_OFF;
    IOLOG( "%s -> 0x%08x", "cbrc_fc_dlvr_field_DLVR_SYNC_RST_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void cbrc_fc_dlvr_field_FC800PMG_CH_EN_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_FC800PMG_CH_EN_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbrc_fc_dlvr_field_FC800PMG_CH_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_FC800PMG_CH_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "cbrc_fc_dlvr_field_FC800PMG_CH_EN_set", N, value );

    /* ((0x00053010 + (N) * 0x4) bits 3) field FC800PMG_CH_EN of register PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG index N=0..11 */
    cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FC800PMG_CH_EN_MSK,
                                                           CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FC800PMG_CH_EN_OFF,
                                                           value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_FC800PMG_CH_EN_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_FC800PMG_CH_EN_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbrc_fc_dlvr_field_FC800PMG_CH_EN_get", N, 11);
    /* ((0x00053010 + (N) * 0x4) bits 3) field FC800PMG_CH_EN of register PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG index N=0..11 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FC800PMG_CH_EN_MSK) >> CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FC800PMG_CH_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cbrc_fc_dlvr_field_FC800PMG_CH_EN_get", N, value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_FCPTD_CH_EN_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_FCPTD_CH_EN_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbrc_fc_dlvr_field_FCPTD_CH_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_FCPTD_CH_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "cbrc_fc_dlvr_field_FCPTD_CH_EN_set", N, value );

    /* ((0x00053010 + (N) * 0x4) bits 2) field FCPTD_CH_EN of register PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG index N=0..11 */
    cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_CH_EN_MSK,
                                                           CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_CH_EN_OFF,
                                                           value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_FCPTD_CH_EN_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_FCPTD_CH_EN_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbrc_fc_dlvr_field_FCPTD_CH_EN_get", N, 11);
    /* ((0x00053010 + (N) * 0x4) bits 2) field FCPTD_CH_EN of register PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG index N=0..11 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_CH_EN_MSK) >> CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_CH_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cbrc_fc_dlvr_field_FCPTD_CH_EN_get", N, value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_LPBK_MUX_SEL_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_LPBK_MUX_SEL_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbrc_fc_dlvr_field_LPBK_MUX_SEL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_LPBK_MUX_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "cbrc_fc_dlvr_field_LPBK_MUX_SEL_set", N, value );

    /* ((0x00053010 + (N) * 0x4) bits 1) field LPBK_MUX_SEL of register PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG index N=0..11 */
    cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_LPBK_MUX_SEL_MSK,
                                                           CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_LPBK_MUX_SEL_OFF,
                                                           value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_LPBK_MUX_SEL_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_LPBK_MUX_SEL_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbrc_fc_dlvr_field_LPBK_MUX_SEL_get", N, 11);
    /* ((0x00053010 + (N) * 0x4) bits 1) field LPBK_MUX_SEL of register PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG index N=0..11 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_LPBK_MUX_SEL_MSK) >> CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_LPBK_MUX_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cbrc_fc_dlvr_field_LPBK_MUX_SEL_get", N, value );

    return value;
}
static INLINE void cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_set( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_set", N, value );

    /* ((0x00053010 + (N) * 0x4) bits 0) field FCPTD_FC800PMG_SEL of register PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG index N=0..11 */
    cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_FC800PMG_SEL_MSK,
                                                           CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_FC800PMG_SEL_OFF,
                                                           value);
}

static INLINE UINT32 cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_get( cbrc_fc_dlvr_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_get", N, 11);
    /* ((0x00053010 + (N) * 0x4) bits 0) field FCPTD_FC800PMG_SEL of register PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG index N=0..11 */
    reg_value = cbrc_fc_dlvr_reg_DLVR_CHANNEL_CFG_REG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_FC800PMG_SEL_MSK) >> CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_FC800PMG_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cbrc_fc_dlvr_field_FCPTD_FC800PMG_SEL_get", N, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBRC_FC_DLVR_IO_INLINE_H */
