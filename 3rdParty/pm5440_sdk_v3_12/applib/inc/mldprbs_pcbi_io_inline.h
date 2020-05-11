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
 *     and register accessor functions for the mldprbs_pcbi block
 *****************************************************************************/
#ifndef _MLDPRBS_PCBI_IO_INLINE_H
#define _MLDPRBS_PCBI_IO_INLINE_H

#include "cbrc_api.h"
#include "mldprbs_pcbi_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MLDPRBS_PCBI_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mldprbs_pcbi
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
} mldprbs_pcbi_buffer_t;
static INLINE void mldprbs_pcbi_buffer_init( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_buffer_init( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mldprbs_pcbi_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mldprbs_pcbi_buffer_flush( mldprbs_pcbi_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_buffer_flush( mldprbs_pcbi_buffer_t *b_ptr )
{
    IOLOG( "mldprbs_pcbi_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mldprbs_pcbi_reg_read( mldprbs_pcbi_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_read( mldprbs_pcbi_buffer_t *b_ptr,
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
static INLINE void mldprbs_pcbi_reg_write( mldprbs_pcbi_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_write( mldprbs_pcbi_buffer_t *b_ptr,
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

static INLINE void mldprbs_pcbi_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 mask,
                                           UINT32 unused_mask,
                                           UINT32 ofs,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_set( mldprbs_pcbi_buffer_t *b_ptr,
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

static INLINE void mldprbs_pcbi_action_on_write_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 mem_type,
                                                           UINT32 reg,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_action_on_write_field_set( mldprbs_pcbi_buffer_t *b_ptr,
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

static INLINE void mldprbs_pcbi_burst_read( mldprbs_pcbi_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_burst_read( mldprbs_pcbi_buffer_t *b_ptr,
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

static INLINE void mldprbs_pcbi_burst_write( mldprbs_pcbi_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_burst_write( mldprbs_pcbi_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 value,
                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                      UINT32 max_count,
                                                      UINT32 *num_failed_polls,
                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_poll( mldprbs_pcbi_buffer_t *b_ptr,
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
 *  register access functions for mldprbs_pcbi
 * ==================================================================================
 */

static INLINE void mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER,
                            value);
}

static INLINE void mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set", mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER,
                            mask,
                            PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read", reg_value);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0,
                            value);
}

static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set", mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0,
                            mask,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read", reg_value);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1,
                            value);
}

static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set", mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1,
                            mask,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read", reg_value);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2,
                            value);
}

static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set", mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2,
                            mask,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read", reg_value);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3,
                            value);
}

static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set", mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3,
                            mask,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read", reg_value);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_GLOBAL_CONFIG_REG,
                            value);
}

static INLINE void mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_field_set", mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_GLOBAL_CONFIG_REG,
                            mask,
                            PMC_MLDPRBS_PCBI_REG_MLDPRBS_GLOBAL_CONFIG_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_MLDPRBS_PCBI_REG_MLDPRBS_GLOBAL_CONFIG_REG);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_read", reg_value);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG(N),
                            value);
}

static INLINE void mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                cbrc_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                cbrc_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_field_set", N, mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG(N),
                            mask,
                            PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_read( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_read( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG(N),
                            value);
}

static INLINE void mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                cbrc_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                cbrc_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_field_set", N, mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG(N),
                            mask,
                            PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_read( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_read( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_CONFIG_REG(N),
                            value);
}

static INLINE void mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_field_set", N, mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_CONFIG_REG(N),
                            mask,
                            PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_CONFIG_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_read( mldprbs_pcbi_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_read( mldprbs_pcbi_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_CONFIG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG(N),
                            value);
}

static INLINE void mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_field_set", N, mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_CONFIG,
                            PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG(N),
                            mask,
                            PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_read( mldprbs_pcbi_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_read( mldprbs_pcbi_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_CONFIG,
                                       PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT,
                            value);
}

static INLINE void mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_action_on_write_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                 cbrc_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_action_on_write_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                 cbrc_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_action_on_write_field_set", mask, ofs, value );
    mldprbs_pcbi_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mldprbs_pcbi_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN,
                            value);
}

static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set", mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN,
                            mask,
                            PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN,
                            value);
}

static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set", mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN,
                            mask,
                            PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E,
                            value);
}

static INLINE void mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_field_set", mask, ofs, value );
    mldprbs_pcbi_field_set( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E,
                            mask,
                            PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_read", reg_value);
    return reg_value;
}

static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT,
                            value);
}

static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                    cbrc_handle_t *h_ptr,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                    cbrc_handle_t *h_ptr,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set", mask, ofs, value );
    mldprbs_pcbi_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mldprbs_pcbi_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_write( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_write", value );
    mldprbs_pcbi_reg_write( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT,
                            value);
}

static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                    cbrc_handle_t *h_ptr,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                    cbrc_handle_t *h_ptr,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set", mask, ofs, value );
    mldprbs_pcbi_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            MEM_TYPE_STATUS,
                                            PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mldprbs_pcbi_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                   cbrc_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                   cbrc_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mldprbs_pcbi_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                   cbrc_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                   cbrc_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mldprbs_pcbi_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE UINT32 mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_read( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mldprbs_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                cbrc_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                cbrc_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mldprbs_pcbi_poll( b_ptr,
                              h_ptr,
                              MEM_TYPE_STATUS,
                              PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL,
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
static INLINE void mldprbs_pcbi_field_STL256_RAM_SD_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_STL256_RAM_SD_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_STL256_RAM_SD_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_STL256_RAM_SD_set", value );

    /* (0x00000cc0 bits 26:24) field STL256_RAM_SD of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( b_ptr,
                                                    h_ptr,
                                                    MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_RAM_SD_MSK,
                                                    MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_RAM_SD_OFF,
                                                    value);
}

static INLINE UINT32 mldprbs_pcbi_field_STL256_RAM_SD_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_STL256_RAM_SD_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cc0 bits 26:24) field STL256_RAM_SD of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    reg_value = mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_RAM_SD_MSK) >> MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_RAM_SD_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_STL256_RAM_SD_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_STL256_RAM_SD_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_STL256_RAM_SD_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL256_RAM_SD_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL256_RAM_SD_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL256_RAM_SD_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cc0 bits 26:24) field STL256_RAM_SD of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
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
        /* (0x00000cc0 bits 26:24) field STL256_RAM_SD of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
        mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_RAM_SD_OFF + subfield_offset),
                                                        MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_RAM_SD_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_STL256_RAM_SD_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_STL256_RAM_SD_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL256_RAM_SD_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL256_RAM_SD_get", stop_bit, 2 );
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
    /* (0x00000cc0 bits 26:24) field STL256_RAM_SD of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    reg_value = mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_RAM_SD_MSK)
                  >> MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_RAM_SD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_RAM_SD_MSK, MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_RAM_SD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL256_RAM_SD_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_STL256_SYNC_RST_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_STL256_SYNC_RST_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_STL256_SYNC_RST_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_STL256_SYNC_RST_set", value );

    /* (0x00000cc0 bits 18:16) field STL256_SYNC_RST of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( b_ptr,
                                                    h_ptr,
                                                    MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_SYNC_RST_MSK,
                                                    MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_SYNC_RST_OFF,
                                                    value);
}

static INLINE UINT32 mldprbs_pcbi_field_STL256_SYNC_RST_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_STL256_SYNC_RST_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cc0 bits 18:16) field STL256_SYNC_RST of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    reg_value = mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_SYNC_RST_MSK) >> MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_SYNC_RST_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_STL256_SYNC_RST_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_STL256_SYNC_RST_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_STL256_SYNC_RST_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL256_SYNC_RST_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL256_SYNC_RST_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL256_SYNC_RST_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cc0 bits 18:16) field STL256_SYNC_RST of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
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
        /* (0x00000cc0 bits 18:16) field STL256_SYNC_RST of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
        mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_SYNC_RST_OFF + subfield_offset),
                                                        MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_SYNC_RST_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_STL256_SYNC_RST_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_STL256_SYNC_RST_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL256_SYNC_RST_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL256_SYNC_RST_get", stop_bit, 2 );
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
    /* (0x00000cc0 bits 18:16) field STL256_SYNC_RST of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    reg_value = mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_SYNC_RST_MSK)
                  >> MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_SYNC_RST_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_SYNC_RST_MSK, MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_SYNC_RST_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL256_SYNC_RST_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_STL256_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_STL256_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_STL256_CLK_EN_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_STL256_CLK_EN_set", value );

    /* (0x00000cc0 bits 6:4) field STL256_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( b_ptr,
                                                    h_ptr,
                                                    MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_CLK_EN_MSK,
                                                    MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_CLK_EN_OFF,
                                                    value);
}

static INLINE UINT32 mldprbs_pcbi_field_STL256_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_STL256_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cc0 bits 6:4) field STL256_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    reg_value = mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_CLK_EN_MSK) >> MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_CLK_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_STL256_CLK_EN_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_STL256_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_STL256_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL256_CLK_EN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL256_CLK_EN_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL256_CLK_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cc0 bits 6:4) field STL256_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
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
        /* (0x00000cc0 bits 6:4) field STL256_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
        mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_CLK_EN_OFF + subfield_offset),
                                                        MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_CLK_EN_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_STL256_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_STL256_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL256_CLK_EN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL256_CLK_EN_get", stop_bit, 2 );
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
    /* (0x00000cc0 bits 6:4) field STL256_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    reg_value = mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_CLK_EN_MSK)
                  >> MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_CLK_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_CLK_EN_MSK, MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_STL256_CLK_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL256_CLK_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SFIS_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SFIS_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SFIS_CLK_EN_set", value, 3);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SFIS_CLK_EN_set", value );

    /* (0x00000cc0 bits 3:2) field SFIS_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( b_ptr,
                                                    h_ptr,
                                                    MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFIS_CLK_EN_MSK,
                                                    MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFIS_CLK_EN_OFF,
                                                    value);
}

static INLINE UINT32 mldprbs_pcbi_field_SFIS_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SFIS_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cc0 bits 3:2) field SFIS_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    reg_value = mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFIS_CLK_EN_MSK) >> MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFIS_CLK_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SFIS_CLK_EN_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_SFIS_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_SFIS_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFIS_CLK_EN_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFIS_CLK_EN_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFIS_CLK_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cc0 bits 3:2) field SFIS_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
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
        /* (0x00000cc0 bits 3:2) field SFIS_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
        mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFIS_CLK_EN_OFF + subfield_offset),
                                                        MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFIS_CLK_EN_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_SFIS_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_SFIS_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFIS_CLK_EN_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFIS_CLK_EN_get", stop_bit, 1 );
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
    /* (0x00000cc0 bits 3:2) field SFIS_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    reg_value = mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFIS_CLK_EN_MSK)
                  >> MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFIS_CLK_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFIS_CLK_EN_MSK, MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFIS_CLK_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFIS_CLK_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SFI51_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SFI51_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SFI51_CLK_EN_set", value, 3);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SFI51_CLK_EN_set", value );

    /* (0x00000cc0 bits 1:0) field SFI51_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( b_ptr,
                                                    h_ptr,
                                                    MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFI51_CLK_EN_MSK,
                                                    MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFI51_CLK_EN_OFF,
                                                    value);
}

static INLINE UINT32 mldprbs_pcbi_field_SFI51_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SFI51_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cc0 bits 1:0) field SFI51_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    reg_value = mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFI51_CLK_EN_MSK) >> MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFI51_CLK_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SFI51_CLK_EN_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_SFI51_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_SFI51_CLK_EN_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFI51_CLK_EN_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFI51_CLK_EN_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFI51_CLK_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cc0 bits 1:0) field SFI51_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
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
        /* (0x00000cc0 bits 1:0) field SFI51_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
        mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFI51_CLK_EN_OFF + subfield_offset),
                                                        MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFI51_CLK_EN_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_SFI51_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_SFI51_CLK_EN_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFI51_CLK_EN_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFI51_CLK_EN_get", stop_bit, 1 );
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
    /* (0x00000cc0 bits 1:0) field SFI51_CLK_EN of register PMC_MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER */
    reg_value = mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFI51_CLK_EN_MSK)
                  >> MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFI51_CLK_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFI51_CLK_EN_MSK, MLDPRBS_PCBI_REG_MLD_CONFIG_REGISTER_BIT_SFI51_CLK_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFI51_CLK_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN0_set", value );

    /* (0x00000cf0 bits 20) field MLDPRBSPCBI_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_MLDPRBSPCBI_IRQ_EN0_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_MLDPRBSPCBI_IRQ_EN0_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf0 bits 20) field MLDPRBSPCBI_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_MLDPRBSPCBI_IRQ_EN0_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_MLDPRBSPCBI_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN0_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_FCDLVR_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_FCDLVR_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN0_set", value );

    /* (0x00000cf0 bits 19) field FCDLVR_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_FCDLVR_IRQ_EN0_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_FCDLVR_IRQ_EN0_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf0 bits 19) field FCDLVR_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_FCDLVR_IRQ_EN0_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_FCDLVR_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN0_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_FCILVR_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_FCILVR_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN0_set", value );

    /* (0x00000cf0 bits 18) field FCILVR_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_FCILVR_IRQ_EN0_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_FCILVR_IRQ_EN0_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf0 bits 18) field FCILVR_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_FCILVR_IRQ_EN0_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_FCILVR_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN0_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SDHPMG1_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SDHPMG1_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN0_set", value );

    /* (0x00000cf0 bits 17) field SDHPMG1_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SDHPMG1_IRQ_EN0_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SDHPMG1_IRQ_EN0_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf0 bits 17) field SDHPMG1_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SDHPMG1_IRQ_EN0_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SDHPMG1_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN0_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SDHPMG0_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SDHPMG0_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN0_set", value );

    /* (0x00000cf0 bits 16) field SDHPMG0_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SDHPMG0_IRQ_EN0_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SDHPMG0_IRQ_EN0_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf0 bits 16) field SDHPMG0_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SDHPMG0_IRQ_EN0_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SDHPMG0_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN0_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_PRBS_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_PRBS_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN0_set", value );

    /* (0x00000cf0 bits 15) field PRBS_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_PRBS_IRQ_EN0_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_PRBS_IRQ_EN0_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf0 bits 15) field PRBS_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_PRBS_IRQ_EN0_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_PRBS_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN0_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_CBRFSGM_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_CBRFSGM_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN0_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN0_set", value );

    /* (0x00000cf0 bits 14:12) field CBRFSGM_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf0 bits 14:12) field CBRFSGM_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN0_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf0 bits 14:12) field CBRFSGM_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
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
        /* (0x00000cf0 bits 14:12) field CBRFSGM_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_get", stop_bit, 2 );
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
    /* (0x00000cf0 bits 14:12) field CBRFSGM_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_STL_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_STL_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN0_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN0_set", value );

    /* (0x00000cf0 bits 10:8) field STL_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_STL_IRQ_EN0_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_STL_IRQ_EN0_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_STL_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_STL_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf0 bits 10:8) field STL_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_STL_IRQ_EN0_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_STL_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN0_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_STL_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_STL_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL_IRQ_EN0_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL_IRQ_EN0_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL_IRQ_EN0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf0 bits 10:8) field STL_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
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
        /* (0x00000cf0 bits 10:8) field STL_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_STL_IRQ_EN0_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_STL_IRQ_EN0_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL_IRQ_EN0_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL_IRQ_EN0_get", stop_bit, 2 );
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
    /* (0x00000cf0 bits 10:8) field STL_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_STL_IRQ_EN0_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_STL_IRQ_EN0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_STL_IRQ_EN0_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_STL_IRQ_EN0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL_IRQ_EN0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SFIS_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SFIS_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN0_set", value, 15);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN0_set", value );

    /* (0x00000cf0 bits 7:4) field SFIS_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFIS_IRQ_EN0_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFIS_IRQ_EN0_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf0 bits 7:4) field SFIS_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFIS_IRQ_EN0_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFIS_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN0_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_SFIS_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_SFIS_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN0_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN0_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFIS_IRQ_EN0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf0 bits 7:4) field SFIS_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
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
        /* (0x00000cf0 bits 7:4) field SFIS_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFIS_IRQ_EN0_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFIS_IRQ_EN0_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN0_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN0_get", stop_bit, 3 );
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
    /* (0x00000cf0 bits 7:4) field SFIS_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFIS_IRQ_EN0_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFIS_IRQ_EN0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFIS_IRQ_EN0_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFIS_IRQ_EN0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFIS_IRQ_EN0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SFI51_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SFI51_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN0_set", value, 3);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN0_set", value );

    /* (0x00000cf0 bits 1:0) field SFI51_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFI51_IRQ_EN0_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFI51_IRQ_EN0_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf0 bits 1:0) field SFI51_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFI51_IRQ_EN0_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFI51_IRQ_EN0_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN0_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_SFI51_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_SFI51_IRQ_EN0_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN0_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN0_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFI51_IRQ_EN0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf0 bits 1:0) field SFI51_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
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
        /* (0x00000cf0 bits 1:0) field SFI51_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFI51_IRQ_EN0_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFI51_IRQ_EN0_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN0_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN0_get", stop_bit, 1 );
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
    /* (0x00000cf0 bits 1:0) field SFI51_IRQ_EN0 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFI51_IRQ_EN0_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFI51_IRQ_EN0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFI51_IRQ_EN0_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG0_BIT_SFI51_IRQ_EN0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFI51_IRQ_EN0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN1_set", value );

    /* (0x00000cf4 bits 20) field MLDPRBSPCBI_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_MLDPRBSPCBI_IRQ_EN1_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_MLDPRBSPCBI_IRQ_EN1_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf4 bits 20) field MLDPRBSPCBI_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_MLDPRBSPCBI_IRQ_EN1_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_MLDPRBSPCBI_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN1_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_FCDLVR_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_FCDLVR_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN1_set", value );

    /* (0x00000cf4 bits 19) field FCDLVR_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_FCDLVR_IRQ_EN1_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_FCDLVR_IRQ_EN1_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf4 bits 19) field FCDLVR_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_FCDLVR_IRQ_EN1_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_FCDLVR_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN1_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_FCILVR_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_FCILVR_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN1_set", value );

    /* (0x00000cf4 bits 18) field FCILVR_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_FCILVR_IRQ_EN1_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_FCILVR_IRQ_EN1_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf4 bits 18) field FCILVR_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_FCILVR_IRQ_EN1_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_FCILVR_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN1_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SDHPMG1_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SDHPMG1_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN1_set", value );

    /* (0x00000cf4 bits 17) field SDHPMG1_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SDHPMG1_IRQ_EN1_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SDHPMG1_IRQ_EN1_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf4 bits 17) field SDHPMG1_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SDHPMG1_IRQ_EN1_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SDHPMG1_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN1_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SDHPMG0_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SDHPMG0_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN1_set", value );

    /* (0x00000cf4 bits 16) field SDHPMG0_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SDHPMG0_IRQ_EN1_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SDHPMG0_IRQ_EN1_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf4 bits 16) field SDHPMG0_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SDHPMG0_IRQ_EN1_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SDHPMG0_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN1_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_PRBS_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_PRBS_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN1_set", value );

    /* (0x00000cf4 bits 15) field PRBS_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_PRBS_IRQ_EN1_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_PRBS_IRQ_EN1_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf4 bits 15) field PRBS_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_PRBS_IRQ_EN1_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_PRBS_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN1_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_CBRFSGM_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_CBRFSGM_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN1_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN1_set", value );

    /* (0x00000cf4 bits 14:12) field CBRFSGM_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf4 bits 14:12) field CBRFSGM_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN1_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf4 bits 14:12) field CBRFSGM_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
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
        /* (0x00000cf4 bits 14:12) field CBRFSGM_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_get", stop_bit, 2 );
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
    /* (0x00000cf4 bits 14:12) field CBRFSGM_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_STL_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_STL_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN1_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN1_set", value );

    /* (0x00000cf4 bits 10:8) field STL_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_STL_IRQ_EN1_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_STL_IRQ_EN1_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_STL_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_STL_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf4 bits 10:8) field STL_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_STL_IRQ_EN1_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_STL_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN1_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_STL_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_STL_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL_IRQ_EN1_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL_IRQ_EN1_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL_IRQ_EN1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf4 bits 10:8) field STL_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
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
        /* (0x00000cf4 bits 10:8) field STL_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_STL_IRQ_EN1_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_STL_IRQ_EN1_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL_IRQ_EN1_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL_IRQ_EN1_get", stop_bit, 2 );
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
    /* (0x00000cf4 bits 10:8) field STL_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_STL_IRQ_EN1_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_STL_IRQ_EN1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_STL_IRQ_EN1_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_STL_IRQ_EN1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL_IRQ_EN1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SFIS_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SFIS_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN1_set", value, 15);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN1_set", value );

    /* (0x00000cf4 bits 7:4) field SFIS_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFIS_IRQ_EN1_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFIS_IRQ_EN1_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf4 bits 7:4) field SFIS_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFIS_IRQ_EN1_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFIS_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN1_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_SFIS_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_SFIS_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN1_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN1_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFIS_IRQ_EN1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf4 bits 7:4) field SFIS_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
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
        /* (0x00000cf4 bits 7:4) field SFIS_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFIS_IRQ_EN1_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFIS_IRQ_EN1_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN1_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN1_get", stop_bit, 3 );
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
    /* (0x00000cf4 bits 7:4) field SFIS_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFIS_IRQ_EN1_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFIS_IRQ_EN1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFIS_IRQ_EN1_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFIS_IRQ_EN1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFIS_IRQ_EN1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SFI51_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SFI51_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN1_set", value, 3);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN1_set", value );

    /* (0x00000cf4 bits 1:0) field SFI51_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFI51_IRQ_EN1_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFI51_IRQ_EN1_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf4 bits 1:0) field SFI51_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFI51_IRQ_EN1_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFI51_IRQ_EN1_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN1_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_SFI51_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_SFI51_IRQ_EN1_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN1_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN1_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFI51_IRQ_EN1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf4 bits 1:0) field SFI51_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
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
        /* (0x00000cf4 bits 1:0) field SFI51_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFI51_IRQ_EN1_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFI51_IRQ_EN1_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN1_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN1_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN1_get", stop_bit, 1 );
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
    /* (0x00000cf4 bits 1:0) field SFI51_IRQ_EN1 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFI51_IRQ_EN1_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFI51_IRQ_EN1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFI51_IRQ_EN1_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG1_BIT_SFI51_IRQ_EN1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFI51_IRQ_EN1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN2_set", value );

    /* (0x00000cf8 bits 20) field MLDPRBSPCBI_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_MLDPRBSPCBI_IRQ_EN2_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_MLDPRBSPCBI_IRQ_EN2_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf8 bits 20) field MLDPRBSPCBI_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_MLDPRBSPCBI_IRQ_EN2_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_MLDPRBSPCBI_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN2_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_FCDLVR_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_FCDLVR_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN2_set", value );

    /* (0x00000cf8 bits 19) field FCDLVR_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_FCDLVR_IRQ_EN2_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_FCDLVR_IRQ_EN2_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf8 bits 19) field FCDLVR_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_FCDLVR_IRQ_EN2_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_FCDLVR_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN2_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_FCILVR_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_FCILVR_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN2_set", value );

    /* (0x00000cf8 bits 18) field FCILVR_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_FCILVR_IRQ_EN2_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_FCILVR_IRQ_EN2_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf8 bits 18) field FCILVR_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_FCILVR_IRQ_EN2_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_FCILVR_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN2_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SDHPMG1_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SDHPMG1_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN2_set", value );

    /* (0x00000cf8 bits 17) field SDHPMG1_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SDHPMG1_IRQ_EN2_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SDHPMG1_IRQ_EN2_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf8 bits 17) field SDHPMG1_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SDHPMG1_IRQ_EN2_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SDHPMG1_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN2_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SDHPMG0_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SDHPMG0_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN2_set", value );

    /* (0x00000cf8 bits 16) field SDHPMG0_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SDHPMG0_IRQ_EN2_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SDHPMG0_IRQ_EN2_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf8 bits 16) field SDHPMG0_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SDHPMG0_IRQ_EN2_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SDHPMG0_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN2_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_PRBS_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_PRBS_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN2_set", value );

    /* (0x00000cf8 bits 15) field PRBS_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_PRBS_IRQ_EN2_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_PRBS_IRQ_EN2_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf8 bits 15) field PRBS_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_PRBS_IRQ_EN2_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_PRBS_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN2_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_CBRFSGM_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_CBRFSGM_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN2_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN2_set", value );

    /* (0x00000cf8 bits 14:12) field CBRFSGM_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf8 bits 14:12) field CBRFSGM_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN2_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf8 bits 14:12) field CBRFSGM_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
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
        /* (0x00000cf8 bits 14:12) field CBRFSGM_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_get", stop_bit, 2 );
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
    /* (0x00000cf8 bits 14:12) field CBRFSGM_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_STL_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_STL_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN2_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN2_set", value );

    /* (0x00000cf8 bits 10:8) field STL_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_STL_IRQ_EN2_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_STL_IRQ_EN2_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_STL_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_STL_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf8 bits 10:8) field STL_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_STL_IRQ_EN2_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_STL_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN2_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_STL_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_STL_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL_IRQ_EN2_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL_IRQ_EN2_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL_IRQ_EN2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf8 bits 10:8) field STL_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
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
        /* (0x00000cf8 bits 10:8) field STL_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_STL_IRQ_EN2_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_STL_IRQ_EN2_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL_IRQ_EN2_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL_IRQ_EN2_get", stop_bit, 2 );
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
    /* (0x00000cf8 bits 10:8) field STL_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_STL_IRQ_EN2_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_STL_IRQ_EN2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_STL_IRQ_EN2_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_STL_IRQ_EN2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL_IRQ_EN2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SFIS_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SFIS_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN2_set", value, 15);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN2_set", value );

    /* (0x00000cf8 bits 7:4) field SFIS_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFIS_IRQ_EN2_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFIS_IRQ_EN2_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf8 bits 7:4) field SFIS_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFIS_IRQ_EN2_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFIS_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN2_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_SFIS_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_SFIS_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN2_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN2_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFIS_IRQ_EN2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf8 bits 7:4) field SFIS_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
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
        /* (0x00000cf8 bits 7:4) field SFIS_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFIS_IRQ_EN2_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFIS_IRQ_EN2_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN2_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN2_get", stop_bit, 3 );
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
    /* (0x00000cf8 bits 7:4) field SFIS_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFIS_IRQ_EN2_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFIS_IRQ_EN2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFIS_IRQ_EN2_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFIS_IRQ_EN2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFIS_IRQ_EN2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SFI51_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SFI51_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN2_set", value, 3);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN2_set", value );

    /* (0x00000cf8 bits 1:0) field SFI51_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFI51_IRQ_EN2_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFI51_IRQ_EN2_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cf8 bits 1:0) field SFI51_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFI51_IRQ_EN2_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFI51_IRQ_EN2_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN2_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_SFI51_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_SFI51_IRQ_EN2_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN2_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN2_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFI51_IRQ_EN2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cf8 bits 1:0) field SFI51_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
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
        /* (0x00000cf8 bits 1:0) field SFI51_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFI51_IRQ_EN2_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFI51_IRQ_EN2_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN2_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN2_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN2_get", stop_bit, 1 );
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
    /* (0x00000cf8 bits 1:0) field SFI51_IRQ_EN2 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFI51_IRQ_EN2_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFI51_IRQ_EN2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFI51_IRQ_EN2_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG2_BIT_SFI51_IRQ_EN2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFI51_IRQ_EN2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN3_set", value );

    /* (0x00000cfc bits 20) field MLDPRBSPCBI_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_MLDPRBSPCBI_IRQ_EN3_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_MLDPRBSPCBI_IRQ_EN3_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cfc bits 20) field MLDPRBSPCBI_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_MLDPRBSPCBI_IRQ_EN3_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_MLDPRBSPCBI_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN3_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_FCDLVR_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_FCDLVR_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN3_set", value );

    /* (0x00000cfc bits 19) field FCDLVR_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_FCDLVR_IRQ_EN3_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_FCDLVR_IRQ_EN3_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cfc bits 19) field FCDLVR_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_FCDLVR_IRQ_EN3_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_FCDLVR_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_FCDLVR_IRQ_EN3_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_FCILVR_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_FCILVR_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN3_set", value );

    /* (0x00000cfc bits 18) field FCILVR_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_FCILVR_IRQ_EN3_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_FCILVR_IRQ_EN3_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cfc bits 18) field FCILVR_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_FCILVR_IRQ_EN3_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_FCILVR_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_FCILVR_IRQ_EN3_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SDHPMG1_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SDHPMG1_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN3_set", value );

    /* (0x00000cfc bits 17) field SDHPMG1_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SDHPMG1_IRQ_EN3_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SDHPMG1_IRQ_EN3_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cfc bits 17) field SDHPMG1_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SDHPMG1_IRQ_EN3_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SDHPMG1_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SDHPMG1_IRQ_EN3_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SDHPMG0_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SDHPMG0_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN3_set", value );

    /* (0x00000cfc bits 16) field SDHPMG0_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SDHPMG0_IRQ_EN3_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SDHPMG0_IRQ_EN3_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cfc bits 16) field SDHPMG0_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SDHPMG0_IRQ_EN3_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SDHPMG0_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SDHPMG0_IRQ_EN3_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_PRBS_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_PRBS_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN3_set", value );

    /* (0x00000cfc bits 15) field PRBS_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_PRBS_IRQ_EN3_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_PRBS_IRQ_EN3_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cfc bits 15) field PRBS_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_PRBS_IRQ_EN3_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_PRBS_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_PRBS_IRQ_EN3_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_CBRFSGM_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_CBRFSGM_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN3_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN3_set", value );

    /* (0x00000cfc bits 14:12) field CBRFSGM_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cfc bits 14:12) field CBRFSGM_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_CBRFSGM_IRQ_EN3_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cfc bits 14:12) field CBRFSGM_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
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
        /* (0x00000cfc bits 14:12) field CBRFSGM_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_get", stop_bit, 2 );
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
    /* (0x00000cfc bits 14:12) field CBRFSGM_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_STL_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_STL_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN3_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN3_set", value );

    /* (0x00000cfc bits 10:8) field STL_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_STL_IRQ_EN3_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_STL_IRQ_EN3_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_STL_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_STL_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cfc bits 10:8) field STL_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_STL_IRQ_EN3_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_STL_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_STL_IRQ_EN3_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_STL_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_STL_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL_IRQ_EN3_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL_IRQ_EN3_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL_IRQ_EN3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cfc bits 10:8) field STL_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
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
        /* (0x00000cfc bits 10:8) field STL_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_STL_IRQ_EN3_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_STL_IRQ_EN3_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_STL_IRQ_EN3_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_STL_IRQ_EN3_get", stop_bit, 2 );
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
    /* (0x00000cfc bits 10:8) field STL_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_STL_IRQ_EN3_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_STL_IRQ_EN3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_STL_IRQ_EN3_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_STL_IRQ_EN3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_STL_IRQ_EN3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SFIS_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SFIS_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN3_set", value, 15);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN3_set", value );

    /* (0x00000cfc bits 7:4) field SFIS_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFIS_IRQ_EN3_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFIS_IRQ_EN3_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cfc bits 7:4) field SFIS_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFIS_IRQ_EN3_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFIS_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SFIS_IRQ_EN3_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_SFIS_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_SFIS_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN3_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN3_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFIS_IRQ_EN3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cfc bits 7:4) field SFIS_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
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
        /* (0x00000cfc bits 7:4) field SFIS_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFIS_IRQ_EN3_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFIS_IRQ_EN3_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN3_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFIS_IRQ_EN3_get", stop_bit, 3 );
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
    /* (0x00000cfc bits 7:4) field SFIS_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFIS_IRQ_EN3_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFIS_IRQ_EN3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFIS_IRQ_EN3_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFIS_IRQ_EN3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFIS_IRQ_EN3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_SFI51_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_SFI51_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN3_set", value, 3);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN3_set", value );

    /* (0x00000cfc bits 1:0) field SFI51_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFI51_IRQ_EN3_MSK,
                                                        MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFI51_IRQ_EN3_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000cfc bits 1:0) field SFI51_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFI51_IRQ_EN3_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFI51_IRQ_EN3_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_SFI51_IRQ_EN3_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_SFI51_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_SFI51_IRQ_EN3_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN3_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN3_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFI51_IRQ_EN3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000cfc bits 1:0) field SFI51_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
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
        /* (0x00000cfc bits 1:0) field SFI51_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
        mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFI51_IRQ_EN3_OFF + subfield_offset),
                                                            MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFI51_IRQ_EN3_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN3_get( mldprbs_pcbi_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN3_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_SFI51_IRQ_EN3_get", stop_bit, 1 );
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
    /* (0x00000cfc bits 1:0) field SFI51_IRQ_EN3 of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3 */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFI51_IRQ_EN3_MSK)
                  >> MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFI51_IRQ_EN3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFI51_IRQ_EN3_MSK, MLDPRBS_PCBI_REG_MLDPRBS_INT_ENABLE_REG3_BIT_SFI51_IRQ_EN3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_SFI51_IRQ_EN3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_MLDPRBS_SYNC_RST_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_MLDPRBS_SYNC_RST_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_MLDPRBS_SYNC_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_MLDPRBS_SYNC_RST_set", value );

    /* (0x00000d00 bits 0) field MLDPRBS_SYNC_RST of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_GLOBAL_CONFIG_REG */
    mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          MLDPRBS_PCBI_REG_MLDPRBS_GLOBAL_CONFIG_REG_BIT_MLDPRBS_SYNC_RST_MSK,
                                                          MLDPRBS_PCBI_REG_MLDPRBS_GLOBAL_CONFIG_REG_BIT_MLDPRBS_SYNC_RST_OFF,
                                                          value);
}

static INLINE UINT32 mldprbs_pcbi_field_MLDPRBS_SYNC_RST_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_MLDPRBS_SYNC_RST_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d00 bits 0) field MLDPRBS_SYNC_RST of register PMC_MLDPRBS_PCBI_REG_MLDPRBS_GLOBAL_CONFIG_REG */
    reg_value = mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_MLDPRBS_GLOBAL_CONFIG_REG_BIT_MLDPRBS_SYNC_RST_MSK) >> MLDPRBS_PCBI_REG_MLDPRBS_GLOBAL_CONFIG_REG_BIT_MLDPRBS_SYNC_RST_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_MLDPRBS_SYNC_RST_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_set", N, value );

    /* ((0x00000c00 + (N) * 0x4) bits 2:1) field MLDPRBS_RX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_RX_MUX_SEL_MSK,
                                                                 MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_RX_MUX_SEL_OFF,
                                                                 value);
}

static INLINE UINT32 mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_get", N, 11);
    /* ((0x00000c00 + (N) * 0x4) bits 2:1) field MLDPRBS_RX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    value = (reg_value & MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_RX_MUX_SEL_MSK) >> MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_RX_MUX_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_get", N, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c00 + (N) * 0x4) bits 2:1) field MLDPRBS_RX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
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
        /* ((0x00000c00 + (N) * 0x4) bits 2:1) field MLDPRBS_RX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
        mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_field_set( b_ptr,
                                                                     h_ptr,
                                                                     N,
                                                                     subfield_mask << (MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_RX_MUX_SEL_OFF + subfield_offset),
                                                                     MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_RX_MUX_SEL_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_get", stop_bit, 1 );
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
    /* ((0x00000c00 + (N) * 0x4) bits 2:1) field MLDPRBS_RX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    field_value = (reg_value & MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_RX_MUX_SEL_MSK)
                  >> MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_RX_MUX_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_RX_MUX_SEL_MSK, MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_RX_MUX_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_set", N, value );

    /* ((0x00000c00 + (N) * 0x4) bits 0) field INGRESS_RX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_INGRESS_RX_MUX_SEL_MSK,
                                                                 MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_INGRESS_RX_MUX_SEL_OFF,
                                                                 value);
}

static INLINE UINT32 mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_get", N, 11);
    /* ((0x00000c00 + (N) * 0x4) bits 0) field INGRESS_RX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    value = (reg_value & MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_INGRESS_RX_MUX_SEL_MSK) >> MLDPRBS_PCBI_REG_RX_MLDPRBS_MULTIPLEXER_REG_BIT_INGRESS_RX_MUX_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_get", N, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_XOFF_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_XOFF_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_XOFF_TX_MUX_SEL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_XOFF_TX_MUX_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "mldprbs_pcbi_field_XOFF_TX_MUX_SEL_set", N, value );

    /* ((0x00000c30 + (N) * 0x4) bits 5:4) field XOFF_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_XOFF_TX_MUX_SEL_MSK,
                                                                 MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_XOFF_TX_MUX_SEL_OFF,
                                                                 value);
}

static INLINE UINT32 mldprbs_pcbi_field_XOFF_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_XOFF_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_XOFF_TX_MUX_SEL_get", N, 11);
    /* ((0x00000c30 + (N) * 0x4) bits 5:4) field XOFF_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_XOFF_TX_MUX_SEL_MSK) >> MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_XOFF_TX_MUX_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mldprbs_pcbi_field_XOFF_TX_MUX_SEL_get", N, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c30 + (N) * 0x4) bits 5:4) field XOFF_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
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
        /* ((0x00000c30 + (N) * 0x4) bits 5:4) field XOFF_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
        mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_field_set( b_ptr,
                                                                     h_ptr,
                                                                     N,
                                                                     subfield_mask << (MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_XOFF_TX_MUX_SEL_OFF + subfield_offset),
                                                                     MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_XOFF_TX_MUX_SEL_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_get", stop_bit, 1 );
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
    /* ((0x00000c30 + (N) * 0x4) bits 5:4) field XOFF_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    field_value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_XOFF_TX_MUX_SEL_MSK)
                  >> MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_XOFF_TX_MUX_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_XOFF_TX_MUX_SEL_MSK, MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_XOFF_TX_MUX_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_LPBK_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_LPBK_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_LPBK_TX_MUX_SEL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_LPBK_TX_MUX_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mldprbs_pcbi_field_LPBK_TX_MUX_SEL_set", N, value );

    /* ((0x00000c30 + (N) * 0x4) bits 3) field LPBK_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_LPBK_TX_MUX_SEL_MSK,
                                                                 MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_LPBK_TX_MUX_SEL_OFF,
                                                                 value);
}

static INLINE UINT32 mldprbs_pcbi_field_LPBK_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_LPBK_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_LPBK_TX_MUX_SEL_get", N, 11);
    /* ((0x00000c30 + (N) * 0x4) bits 3) field LPBK_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_LPBK_TX_MUX_SEL_MSK) >> MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_LPBK_TX_MUX_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mldprbs_pcbi_field_LPBK_TX_MUX_SEL_get", N, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_set", N, value );

    /* ((0x00000c30 + (N) * 0x4) bits 2:1) field EGRESS_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_EGRESS_TX_MUX_SEL_MSK,
                                                                 MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_EGRESS_TX_MUX_SEL_OFF,
                                                                 value);
}

static INLINE UINT32 mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_get", N, 11);
    /* ((0x00000c30 + (N) * 0x4) bits 2:1) field EGRESS_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_EGRESS_TX_MUX_SEL_MSK) >> MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_EGRESS_TX_MUX_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_get", N, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c30 + (N) * 0x4) bits 2:1) field EGRESS_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
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
        /* ((0x00000c30 + (N) * 0x4) bits 2:1) field EGRESS_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
        mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_field_set( b_ptr,
                                                                     h_ptr,
                                                                     N,
                                                                     subfield_mask << (MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_EGRESS_TX_MUX_SEL_OFF + subfield_offset),
                                                                     MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_EGRESS_TX_MUX_SEL_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_get", stop_bit, 1 );
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
    /* ((0x00000c30 + (N) * 0x4) bits 2:1) field EGRESS_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    field_value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_EGRESS_TX_MUX_SEL_MSK)
                  >> MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_EGRESS_TX_MUX_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_EGRESS_TX_MUX_SEL_MSK, MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_EGRESS_TX_MUX_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_set", N, value );

    /* ((0x00000c30 + (N) * 0x4) bits 0) field MLDPRBS_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_TX_MUX_SEL_MSK,
                                                                 MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_TX_MUX_SEL_OFF,
                                                                 value);
}

static INLINE UINT32 mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_get", N, 11);
    /* ((0x00000c30 + (N) * 0x4) bits 0) field MLDPRBS_TX_MUX_SEL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_TX_MUX_SEL_MSK) >> MLDPRBS_PCBI_REG_TX_MLDPRBS_MULTIPLEXER_REG_BIT_MLDPRBS_TX_MUX_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_get", N, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set", N, value );

    /* ((0x00000c60 + (N) * 0x4) bits 0) field RX_FIFO_SYNC_RST of register PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_CONFIG_REG index N=0..11 */
    mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            MLDPRBS_PCBI_REG_RX_MLDPRBS_CONFIG_REG_BIT_RX_FIFO_SYNC_RST_MSK,
                                                            MLDPRBS_PCBI_REG_RX_MLDPRBS_CONFIG_REG_BIT_RX_FIFO_SYNC_RST_OFF,
                                                            value);
}

static INLINE UINT32 mldprbs_pcbi_field_RX_FIFO_SYNC_RST_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_RX_FIFO_SYNC_RST_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_RX_FIFO_SYNC_RST_get", N, 11);
    /* ((0x00000c60 + (N) * 0x4) bits 0) field RX_FIFO_SYNC_RST of register PMC_MLDPRBS_PCBI_REG_RX_MLDPRBS_CONFIG_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & MLDPRBS_PCBI_REG_RX_MLDPRBS_CONFIG_REG_BIT_RX_FIFO_SYNC_RST_MSK) >> MLDPRBS_PCBI_REG_RX_MLDPRBS_CONFIG_REG_BIT_RX_FIFO_SYNC_RST_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mldprbs_pcbi_field_RX_FIFO_SYNC_RST_get", N, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_set", N, 11);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_set", N, value );

    /* ((0x00000c90 + (N) * 0x4) bits 8:4) field TX_FIFO_THRSHD_VAL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
    mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_THRSHD_VAL_MSK,
                                                            MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_THRSHD_VAL_OFF,
                                                            value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_get", N, 11);
    /* ((0x00000c90 + (N) * 0x4) bits 8:4) field TX_FIFO_THRSHD_VAL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_THRSHD_VAL_MSK) >> MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_THRSHD_VAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_get", N, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c90 + (N) * 0x4) bits 8:4) field TX_FIFO_THRSHD_VAL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
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
        /* ((0x00000c90 + (N) * 0x4) bits 8:4) field TX_FIFO_THRSHD_VAL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
        mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_field_set( b_ptr,
                                                                h_ptr,
                                                                N,
                                                                subfield_mask << (MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_THRSHD_VAL_OFF + subfield_offset),
                                                                MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_THRSHD_VAL_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_get", stop_bit, 4 );
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
    /* ((0x00000c90 + (N) * 0x4) bits 8:4) field TX_FIFO_THRSHD_VAL of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    field_value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_THRSHD_VAL_MSK)
                  >> MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_THRSHD_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_THRSHD_VAL_MSK, MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_THRSHD_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_FIFO_RAM_ERR_INJECT_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_FIFO_RAM_ERR_INJECT_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_TX_FIFO_RAM_ERR_INJECT_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_FIFO_RAM_ERR_INJECT_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "mldprbs_pcbi_field_TX_FIFO_RAM_ERR_INJECT_set", N, value );

    /* ((0x00000c90 + (N) * 0x4) bits 3:2) field TX_FIFO_RAM_ERR_INJECT of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
    mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_ERR_INJECT_MSK,
                                                            MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_ERR_INJECT_OFF,
                                                            value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_FIFO_RAM_ERR_INJECT_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_FIFO_RAM_ERR_INJECT_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_TX_FIFO_RAM_ERR_INJECT_get", N, 11);
    /* ((0x00000c90 + (N) * 0x4) bits 3:2) field TX_FIFO_RAM_ERR_INJECT of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_ERR_INJECT_MSK) >> MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_ERR_INJECT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mldprbs_pcbi_field_TX_FIFO_RAM_ERR_INJECT_get", N, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c90 + (N) * 0x4) bits 3:2) field TX_FIFO_RAM_ERR_INJECT of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
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
        /* ((0x00000c90 + (N) * 0x4) bits 3:2) field TX_FIFO_RAM_ERR_INJECT of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
        mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_field_set( b_ptr,
                                                                h_ptr,
                                                                N,
                                                                subfield_mask << (MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_ERR_INJECT_OFF + subfield_offset),
                                                                MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_ERR_INJECT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                          cbrc_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                                          cbrc_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_get", stop_bit, 1 );
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
    /* ((0x00000c90 + (N) * 0x4) bits 3:2) field TX_FIFO_RAM_ERR_INJECT of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    field_value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_ERR_INJECT_MSK)
                  >> MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_ERR_INJECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_ERR_INJECT_MSK, MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_ERR_INJECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_FIFO_RAM_SD_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_FIFO_RAM_SD_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_TX_FIFO_RAM_SD_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_FIFO_RAM_SD_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mldprbs_pcbi_field_TX_FIFO_RAM_SD_set", N, value );

    /* ((0x00000c90 + (N) * 0x4) bits 1) field TX_FIFO_RAM_SD of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
    mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_SD_MSK,
                                                            MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_SD_OFF,
                                                            value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_FIFO_RAM_SD_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_FIFO_RAM_SD_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_TX_FIFO_RAM_SD_get", N, 11);
    /* ((0x00000c90 + (N) * 0x4) bits 1) field TX_FIFO_RAM_SD of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_SD_MSK) >> MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_RAM_SD_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mldprbs_pcbi_field_TX_FIFO_RAM_SD_get", N, value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set", N, value );

    /* ((0x00000c90 + (N) * 0x4) bits 0) field TX_FIFO_SYNC_RST of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
    mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_SYNC_RST_MSK,
                                                            MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_SYNC_RST_OFF,
                                                            value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_FIFO_SYNC_RST_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_FIFO_SYNC_RST_get( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mldprbs_pcbi_field_TX_FIFO_SYNC_RST_get", N, 11);
    /* ((0x00000c90 + (N) * 0x4) bits 0) field TX_FIFO_SYNC_RST of register PMC_MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG index N=0..11 */
    reg_value = mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_SYNC_RST_MSK) >> MLDPRBS_PCBI_REG_TX_MLDPRBS_CONFIG_REG_BIT_TX_FIFO_SYNC_RST_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mldprbs_pcbi_field_TX_FIFO_SYNC_RST_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */
static INLINE void mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_set_to_clear", value );

    /* (0x00000d28 bits 1) field TIP_NEG_EDGE_INT_I of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT */
    mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_NEG_EDGE_INT_I_MSK,
                                                                  MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_NEG_EDGE_INT_I_OFF,
                                                                  value);
}

static INLINE UINT32 mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d28 bits 1) field TIP_NEG_EDGE_INT_I of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT */
    reg_value = mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_NEG_EDGE_INT_I_MSK) >> MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_NEG_EDGE_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_poll", value );

    /* (0x00000d28 bits 1) field TIP_NEG_EDGE_INT_I of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT */
    return mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_poll( b_ptr,
                                                    h_ptr,
                                                    MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_NEG_EDGE_INT_I_MSK,
                                                    (value<<MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_NEG_EDGE_INT_I_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_set_to_clear", value );

    /* (0x00000d28 bits 0) field TIP_POS_EDGE_INT_I of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT */
    mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_POS_EDGE_INT_I_MSK,
                                                                  MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_POS_EDGE_INT_I_OFF,
                                                                  value);
}

static INLINE UINT32 mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d28 bits 0) field TIP_POS_EDGE_INT_I of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT */
    reg_value = mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_POS_EDGE_INT_I_MSK) >> MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_POS_EDGE_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_poll", value );

    /* (0x00000d28 bits 0) field TIP_POS_EDGE_INT_I of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT */
    return mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_poll( b_ptr,
                                                    h_ptr,
                                                    MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_POS_EDGE_INT_I_MSK,
                                                    (value<<MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_BIT_TIP_POS_EDGE_INT_I_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 11) field TX_XFI_FIFO_11_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_11_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_11_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 11) field TX_XFI_FIFO_11_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_11_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_11_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 10) field TX_XFI_FIFO_10_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_10_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_10_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 10) field TX_XFI_FIFO_10_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_10_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_10_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 9) field TX_XFI_FIFO_9_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_9_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_9_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 9) field TX_XFI_FIFO_9_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_9_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_9_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 8) field TX_XFI_FIFO_8_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_8_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_8_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 8) field TX_XFI_FIFO_8_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_8_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_8_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 7) field TX_XFI_FIFO_7_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_7_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_7_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 7) field TX_XFI_FIFO_7_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_7_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_7_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 6) field TX_XFI_FIFO_6_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_6_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_6_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 6) field TX_XFI_FIFO_6_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_6_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_6_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 5) field TX_XFI_FIFO_5_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_5_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_5_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 5) field TX_XFI_FIFO_5_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_5_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_5_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 4) field TX_XFI_FIFO_4_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_4_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_4_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 4) field TX_XFI_FIFO_4_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_4_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_4_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 3) field TX_XFI_FIFO_3_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_3_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_3_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 3) field TX_XFI_FIFO_3_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_3_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_3_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 2) field TX_XFI_FIFO_2_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_2_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_2_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 2) field TX_XFI_FIFO_2_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_2_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_2_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 1) field TX_XFI_FIFO_1_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_1_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_1_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 1) field TX_XFI_FIFO_1_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_1_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_1_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_E_set", value );

    /* (0x00000d14 bits 0) field TX_XFI_FIFO_0_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_0_UNFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_0_UNFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d14 bits 0) field TX_XFI_FIFO_0_UNFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_0_UNFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_EN_BIT_TX_XFI_FIFO_0_UNFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 11) field TX_XFI_FIFO_11_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_11_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_11_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 11) field TX_XFI_FIFO_11_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_11_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_11_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                     cbrc_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 10) field TX_XFI_FIFO_10_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_10_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_10_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 10) field TX_XFI_FIFO_10_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_10_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_10_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 9) field TX_XFI_FIFO_9_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_9_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_9_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 9) field TX_XFI_FIFO_9_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_9_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_9_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 8) field TX_XFI_FIFO_8_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_8_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_8_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 8) field TX_XFI_FIFO_8_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_8_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_8_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 7) field TX_XFI_FIFO_7_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_7_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_7_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 7) field TX_XFI_FIFO_7_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_7_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_7_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 6) field TX_XFI_FIFO_6_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_6_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_6_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 6) field TX_XFI_FIFO_6_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_6_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_6_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 5) field TX_XFI_FIFO_5_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_5_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_5_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 5) field TX_XFI_FIFO_5_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_5_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_5_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 4) field TX_XFI_FIFO_4_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_4_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_4_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 4) field TX_XFI_FIFO_4_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_4_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_4_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 3) field TX_XFI_FIFO_3_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_3_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_3_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 3) field TX_XFI_FIFO_3_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_3_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_3_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 2) field TX_XFI_FIFO_2_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_2_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_2_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 2) field TX_XFI_FIFO_2_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_2_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_2_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 1) field TX_XFI_FIFO_1_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_1_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_1_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 1) field TX_XFI_FIFO_1_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_1_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_1_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_E_set", value );

    /* (0x00000d20 bits 0) field TX_XFI_FIFO_0_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_0_OVFL_INT_E_MSK,
                                                        MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_0_OVFL_INT_E_OFF,
                                                        value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d20 bits 0) field TX_XFI_FIFO_0_OVFL_INT_E of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_0_OVFL_INT_E_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_EN_BIT_TX_XFI_FIFO_0_OVFL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TIP_NEG_EDGE_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TIP_NEG_EDGE_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TIP_NEG_EDGE_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TIP_NEG_EDGE_INT_E_set", value );

    /* (0x00000d2c bits 1) field TIP_NEG_EDGE_INT_E of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E */
    mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_field_set( b_ptr,
                                                    h_ptr,
                                                    MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E_BIT_TIP_NEG_EDGE_INT_E_MSK,
                                                    MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E_BIT_TIP_NEG_EDGE_INT_E_OFF,
                                                    value);
}

static INLINE UINT32 mldprbs_pcbi_field_TIP_NEG_EDGE_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TIP_NEG_EDGE_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d2c bits 1) field TIP_NEG_EDGE_INT_E of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E */
    reg_value = mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E_BIT_TIP_NEG_EDGE_INT_E_MSK) >> MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E_BIT_TIP_NEG_EDGE_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TIP_NEG_EDGE_INT_E_get", value );

    return value;
}
static INLINE void mldprbs_pcbi_field_TIP_POS_EDGE_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TIP_POS_EDGE_INT_E_set( mldprbs_pcbi_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TIP_POS_EDGE_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TIP_POS_EDGE_INT_E_set", value );

    /* (0x00000d2c bits 0) field TIP_POS_EDGE_INT_E of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E */
    mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_field_set( b_ptr,
                                                    h_ptr,
                                                    MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E_BIT_TIP_POS_EDGE_INT_E_MSK,
                                                    MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E_BIT_TIP_POS_EDGE_INT_E_OFF,
                                                    value);
}

static INLINE UINT32 mldprbs_pcbi_field_TIP_POS_EDGE_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TIP_POS_EDGE_INT_E_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d2c bits 0) field TIP_POS_EDGE_INT_E of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E */
    reg_value = mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E_BIT_TIP_POS_EDGE_INT_E_MSK) >> MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_E_BIT_TIP_POS_EDGE_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TIP_POS_EDGE_INT_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 11) field TX_XFI_FIFO_11_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_11_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_11_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 11) field TX_XFI_FIFO_11_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_11_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_11_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 11) field TX_XFI_FIFO_11_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_11_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_11_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 10) field TX_XFI_FIFO_10_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_10_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_10_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 10) field TX_XFI_FIFO_10_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_10_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_10_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 10) field TX_XFI_FIFO_10_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_10_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_10_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 9) field TX_XFI_FIFO_9_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_9_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_9_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 9) field TX_XFI_FIFO_9_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_9_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_9_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 9) field TX_XFI_FIFO_9_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_9_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_9_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 8) field TX_XFI_FIFO_8_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_8_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_8_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 8) field TX_XFI_FIFO_8_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_8_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_8_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 8) field TX_XFI_FIFO_8_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_8_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_8_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 7) field TX_XFI_FIFO_7_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_7_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_7_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 7) field TX_XFI_FIFO_7_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_7_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_7_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 7) field TX_XFI_FIFO_7_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_7_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_7_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 6) field TX_XFI_FIFO_6_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_6_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_6_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 6) field TX_XFI_FIFO_6_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_6_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_6_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 6) field TX_XFI_FIFO_6_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_6_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_6_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 5) field TX_XFI_FIFO_5_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_5_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_5_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 5) field TX_XFI_FIFO_5_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_5_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_5_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 5) field TX_XFI_FIFO_5_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_5_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_5_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 4) field TX_XFI_FIFO_4_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_4_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_4_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 4) field TX_XFI_FIFO_4_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_4_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_4_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 4) field TX_XFI_FIFO_4_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_4_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_4_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 3) field TX_XFI_FIFO_3_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_3_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_3_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 3) field TX_XFI_FIFO_3_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_3_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_3_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 3) field TX_XFI_FIFO_3_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_3_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_3_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 2) field TX_XFI_FIFO_2_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_2_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_2_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 2) field TX_XFI_FIFO_2_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_2_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_2_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 2) field TX_XFI_FIFO_2_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_2_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_2_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 1) field TX_XFI_FIFO_1_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_1_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_1_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 1) field TX_XFI_FIFO_1_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_1_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_1_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 1) field TX_XFI_FIFO_1_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_1_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_1_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_set_to_clear", value );

    /* (0x00000d10 bits 0) field TX_XFI_FIFO_0_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_0_UNFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_0_UNFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d10 bits 0) field TX_XFI_FIFO_0_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_0_UNFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_0_UNFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_poll", value );

    /* (0x00000d10 bits 0) field TX_XFI_FIFO_0_UNFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_0_UNFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_BIT_TX_XFI_FIFO_0_UNFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 11) field TX_XFI_FIFO_11_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_11_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_11_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 11) field TX_XFI_FIFO_11_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_11_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_11_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 11) field TX_XFI_FIFO_11_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_11_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_11_OVFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 10) field TX_XFI_FIFO_10_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_10_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_10_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 10) field TX_XFI_FIFO_10_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_10_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_10_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 10) field TX_XFI_FIFO_10_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_10_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_10_OVFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 9) field TX_XFI_FIFO_9_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_9_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_9_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 9) field TX_XFI_FIFO_9_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_9_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_9_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 9) field TX_XFI_FIFO_9_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_9_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_9_OVFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 8) field TX_XFI_FIFO_8_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_8_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_8_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 8) field TX_XFI_FIFO_8_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_8_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_8_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 8) field TX_XFI_FIFO_8_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_8_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_8_OVFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 7) field TX_XFI_FIFO_7_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_7_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_7_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 7) field TX_XFI_FIFO_7_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_7_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_7_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 7) field TX_XFI_FIFO_7_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_7_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_7_OVFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 6) field TX_XFI_FIFO_6_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_6_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_6_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 6) field TX_XFI_FIFO_6_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_6_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_6_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 6) field TX_XFI_FIFO_6_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_6_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_6_OVFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 5) field TX_XFI_FIFO_5_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_5_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_5_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 5) field TX_XFI_FIFO_5_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_5_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_5_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 5) field TX_XFI_FIFO_5_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_5_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_5_OVFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 4) field TX_XFI_FIFO_4_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_4_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_4_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 4) field TX_XFI_FIFO_4_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_4_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_4_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 4) field TX_XFI_FIFO_4_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_4_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_4_OVFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 3) field TX_XFI_FIFO_3_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_3_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_3_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 3) field TX_XFI_FIFO_3_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_3_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_3_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 3) field TX_XFI_FIFO_3_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_3_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_3_OVFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 2) field TX_XFI_FIFO_2_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_2_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_2_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 2) field TX_XFI_FIFO_2_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_2_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_2_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 2) field TX_XFI_FIFO_2_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_2_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_2_OVFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 1) field TX_XFI_FIFO_1_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_1_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_1_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 1) field TX_XFI_FIFO_1_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_1_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_1_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 1) field TX_XFI_FIFO_1_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_1_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_1_OVFL_INT_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_set_to_clear( mldprbs_pcbi_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_set_to_clear", value );

    /* (0x00000d1c bits 0) field TX_XFI_FIFO_0_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_0_OVFL_INT_I_MSK,
                                                                     MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_0_OVFL_INT_I_OFF,
                                                                     value);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d1c bits 0) field TX_XFI_FIFO_0_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_0_OVFL_INT_I_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_0_OVFL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_poll", value );

    /* (0x00000d1c bits 0) field TX_XFI_FIFO_0_OVFL_INT_I of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll( b_ptr,
                                                       h_ptr,
                                                       MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_0_OVFL_INT_I_MSK,
                                                       (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_BIT_TX_XFI_FIFO_0_OVFL_INT_I_OFF),
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
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 11) field TX_XFI_FIFO_11_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_11_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_11_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 11) field TX_XFI_FIFO_11_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_11_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_11_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 10) field TX_XFI_FIFO_10_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_10_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_10_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 10) field TX_XFI_FIFO_10_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_10_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_10_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 9) field TX_XFI_FIFO_9_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_9_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_9_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 9) field TX_XFI_FIFO_9_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_9_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_9_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 8) field TX_XFI_FIFO_8_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_8_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_8_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 8) field TX_XFI_FIFO_8_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_8_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_8_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 7) field TX_XFI_FIFO_7_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_7_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_7_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 7) field TX_XFI_FIFO_7_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_7_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_7_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 6) field TX_XFI_FIFO_6_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_6_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_6_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 6) field TX_XFI_FIFO_6_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_6_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_6_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 5) field TX_XFI_FIFO_5_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_5_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_5_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 5) field TX_XFI_FIFO_5_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_5_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_5_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 4) field TX_XFI_FIFO_4_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_4_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_4_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 4) field TX_XFI_FIFO_4_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_4_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_4_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 3) field TX_XFI_FIFO_3_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_3_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_3_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 3) field TX_XFI_FIFO_3_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_3_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_3_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 2) field TX_XFI_FIFO_2_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_2_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_2_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 2) field TX_XFI_FIFO_2_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_2_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_2_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 1) field TX_XFI_FIFO_1_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_1_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_1_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 1) field TX_XFI_FIFO_1_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_1_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_1_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d18 bits 0) field TX_XFI_FIFO_0_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_0_UNFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_0_UNFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_V_poll", value );

    /* (0x00000d18 bits 0) field TX_XFI_FIFO_0_UNFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_0_UNFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_UNFL_INT_VAL_BIT_TX_XFI_FIFO_0_UNFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 11) field TX_XFI_FIFO_11_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_11_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_11_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 11) field TX_XFI_FIFO_11_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_11_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_11_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 10) field TX_XFI_FIFO_10_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_10_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_10_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                      cbrc_handle_t *h_ptr,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 10) field TX_XFI_FIFO_10_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_10_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_10_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 9) field TX_XFI_FIFO_9_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_9_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_9_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 9) field TX_XFI_FIFO_9_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_9_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_9_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 8) field TX_XFI_FIFO_8_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_8_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_8_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 8) field TX_XFI_FIFO_8_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_8_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_8_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 7) field TX_XFI_FIFO_7_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_7_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_7_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 7) field TX_XFI_FIFO_7_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_7_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_7_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 6) field TX_XFI_FIFO_6_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_6_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_6_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 6) field TX_XFI_FIFO_6_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_6_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_6_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 5) field TX_XFI_FIFO_5_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_5_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_5_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 5) field TX_XFI_FIFO_5_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_5_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_5_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 4) field TX_XFI_FIFO_4_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_4_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_4_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 4) field TX_XFI_FIFO_4_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_4_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_4_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 3) field TX_XFI_FIFO_3_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_3_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_3_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 3) field TX_XFI_FIFO_3_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_3_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_3_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 2) field TX_XFI_FIFO_2_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_2_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_2_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 2) field TX_XFI_FIFO_2_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_2_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_2_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 1) field TX_XFI_FIFO_1_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_1_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_1_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 1) field TX_XFI_FIFO_1_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_1_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_1_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d24 bits 0) field TX_XFI_FIFO_0_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    reg_value = mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_0_OVFL_INT_V_MSK) >> MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_0_OVFL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                                     cbrc_handle_t *h_ptr,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_V_poll", value );

    /* (0x00000d24 bits 0) field TX_XFI_FIFO_0_OVFL_INT_V of register PMC_MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL */
    return mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll( b_ptr,
                                                           h_ptr,
                                                           MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_0_OVFL_INT_V_MSK,
                                                           (value<<MLDPRBS_PCBI_REG_TX_XFI_FIFO_OVFL_INT_VAL_BIT_TX_XFI_FIFO_0_OVFL_INT_V_OFF),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TIP_NEG_EDGE_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TIP_NEG_EDGE_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d30 bits 1) field TIP_NEG_EDGE_INT_V of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL */
    reg_value = mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL_BIT_TIP_NEG_EDGE_INT_V_MSK) >> MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL_BIT_TIP_NEG_EDGE_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TIP_NEG_EDGE_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_NEG_EDGE_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_NEG_EDGE_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TIP_NEG_EDGE_INT_V_poll", value );

    /* (0x00000d30 bits 1) field TIP_NEG_EDGE_INT_V of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL */
    return mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_poll( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL_BIT_TIP_NEG_EDGE_INT_V_MSK,
                                                        (value<<MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL_BIT_TIP_NEG_EDGE_INT_V_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 mldprbs_pcbi_field_TIP_POS_EDGE_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mldprbs_pcbi_field_TIP_POS_EDGE_INT_V_get( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000d30 bits 0) field TIP_POS_EDGE_INT_V of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL */
    reg_value = mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL_BIT_TIP_POS_EDGE_INT_V_MSK) >> MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL_BIT_TIP_POS_EDGE_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "mldprbs_pcbi_field_TIP_POS_EDGE_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_POS_EDGE_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_POS_EDGE_INT_V_poll( mldprbs_pcbi_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mldprbs_pcbi_field_TIP_POS_EDGE_INT_V_poll", value );

    /* (0x00000d30 bits 0) field TIP_POS_EDGE_INT_V of register PMC_MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL */
    return mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_poll( b_ptr,
                                                        h_ptr,
                                                        MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL_BIT_TIP_POS_EDGE_INT_V_MSK,
                                                        (value<<MLDPRBS_PCBI_REG_CBRC_TIP_EDGE_INT_VAL_BIT_TIP_POS_EDGE_INT_V_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MLDPRBS_PCBI_IO_INLINE_H */
