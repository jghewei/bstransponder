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
 *     and register accessor functions for the coreotn_fmf block
 *****************************************************************************/
#ifndef _COREOTN_FMF_IO_INLINE_H
#define _COREOTN_FMF_IO_INLINE_H

#include "coreotn_loc.h"
#include "coreotn_fmf_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define COREOTN_FMF_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for coreotn_fmf
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
    coreotn_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} coreotn_fmf_buffer_t;
static INLINE void coreotn_fmf_buffer_init( coreotn_fmf_buffer_t *b_ptr,
                                            coreotn_handle_t *h_ptr,
                                            UINT32 A ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_buffer_init( coreotn_fmf_buffer_t *b_ptr,
                                            coreotn_handle_t *h_ptr,
                                            UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x100000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "coreotn_fmf_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void coreotn_fmf_buffer_flush( coreotn_fmf_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_buffer_flush( coreotn_fmf_buffer_t *b_ptr )
{
    IOLOG( "coreotn_fmf_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 coreotn_fmf_reg_read( coreotn_fmf_buffer_t *b_ptr,
                                           coreotn_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_read( coreotn_fmf_buffer_t *b_ptr,
                                           coreotn_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x100000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x100000) + reg);
    return value;
}
static INLINE void coreotn_fmf_reg_write( coreotn_fmf_buffer_t *b_ptr,
                                          coreotn_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_write( coreotn_fmf_buffer_t *b_ptr,
                                          coreotn_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x100000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x100000) + reg,
                         value);
}

static INLINE void coreotn_fmf_field_set( coreotn_fmf_buffer_t *b_ptr,
                                          coreotn_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 mask,
                                          UINT32 unused_mask,
                                          UINT32 ofs,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_set( coreotn_fmf_buffer_t *b_ptr,
                                          coreotn_handle_t *h_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x100000),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x100000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x100000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void coreotn_fmf_action_on_write_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mem_type,
                                                          UINT32 reg,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_action_on_write_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*0x100000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x100000) + reg,
                         value<<ofs);
}

static INLINE void coreotn_fmf_burst_read( coreotn_fmf_buffer_t *b_ptr,
                                           coreotn_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_burst_read( coreotn_fmf_buffer_t *b_ptr,
                                           coreotn_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x100000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x100000) + reg,
                          len,
                          value);
}

static INLINE void coreotn_fmf_burst_write( coreotn_fmf_buffer_t *b_ptr,
                                            coreotn_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_burst_write( coreotn_fmf_buffer_t *b_ptr,
                                            coreotn_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x100000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x100000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_poll( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 value,
                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                     UINT32 max_count,
                                                     UINT32 *num_failed_polls,
                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_poll( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x100000),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x100000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for coreotn_fmf
 * ==================================================================================
 */

static INLINE void coreotn_fmf_reg_SW_RESET_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_SW_RESET_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_SW_RESET_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_SW_RESET_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_SW_RESET_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_SW_RESET_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_SW_RESET_REG_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_SW_RESET_REG,
                           mask,
                           PMC_COREOTN_FMF_REG_SW_RESET_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_SW_RESET_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_SW_RESET_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_SW_RESET_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_SW_RESET_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_CLK_GATING_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_CLK_GATING_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_CLK_GATING_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_CLK_GATING_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_CLK_GATING_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_CLK_GATING_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_CLK_GATING_REG_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_CLK_GATING_REG,
                           mask,
                           PMC_COREOTN_FMF_REG_CLK_GATING_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_CLK_GATING_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_CLK_GATING_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_CLK_GATING_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_CLK_GATING_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_RAM_LOW_POWER_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_RAM_LOW_POWER_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_RAM_LOW_POWER_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG,
                           mask,
                           PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_RAM_LOW_POWER_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_RAM_LOW_POWER_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_RAM_LOW_POWER_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_DCI_CONFIG_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_DCI_CONFIG_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_DCI_CONFIG_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_DCI_CONFIG_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_DCI_CONFIG_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_DCI_CONFIG_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_DCI_CONFIG_REG_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_DCI_CONFIG_REG,
                           mask,
                           PMC_COREOTN_FMF_REG_DCI_CONFIG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_DCI_CONFIG_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_DCI_CONFIG_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_DCI_CONFIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_DCI_CONFIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG,
                           mask,
                           PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_TSB_CONFIG_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_TSB_CONFIG_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_TSB_CONFIG_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_TSB_CONFIG_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_TSB_CONFIG_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_TSB_CONFIG_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_TSB_CONFIG_REG_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_TSB_CONFIG_REG,
                           mask,
                           PMC_COREOTN_FMF_REG_TSB_CONFIG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_TSB_CONFIG_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_TSB_CONFIG_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_TSB_CONFIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_TSB_CONFIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1,
                           value);
}

static INLINE void coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1,
                           mask,
                           PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2,
                           value);
}

static INLINE void coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2,
                           mask,
                           PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1,
                           value);
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1,
                           mask,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2,
                           value);
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2,
                           mask,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3,
                           value);
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3,
                           mask,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4,
                           value);
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4,
                           mask,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5,
                           value);
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5,
                           mask,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_write( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6,
                           value);
}

static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6,
                           mask,
                           PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_GPO_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                  coreotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_GPO_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                  coreotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_GPO_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_GPO_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_GPO_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_GPO_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_GPO_REG_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_GPO_REG,
                           mask,
                           PMC_COREOTN_FMF_REG_GPO_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_GPO_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_GPO_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_GPO_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_GPO_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                        coreotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                        coreotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG,
                           mask,
                           PMC_COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                        coreotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                        coreotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG,
                           mask,
                           PMC_COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_PRBS_RESEED_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_RESEED_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_PRBS_RESEED_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_PRBS_RESEED_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_PRBS_RESEED_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_RESEED_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_PRBS_RESEED_REG_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_PRBS_RESEED_REG,
                           mask,
                           PMC_COREOTN_FMF_REG_PRBS_RESEED_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_PRBS_RESEED_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_PRBS_RESEED_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_PRBS_RESEED_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_PRBS_RESEED_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_write( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32  N,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_write( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32  N,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG(N),
                           value);
}

static INLINE void coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                         coreotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                         coreotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_field_set", A, N, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG(N),
                           mask,
                           PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_read( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_read( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32  N )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_write( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32  N,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_write( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32  N,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG(N),
                           value);
}

static INLINE void coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                        coreotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                        coreotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_field_set", A, N, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG(N),
                           mask,
                           PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_read( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_read( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32  N )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_CONFIG,
                                      PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_STATUS,
                           PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_field_set", A, mask, ofs, value );
    coreotn_fmf_field_set( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_STATUS,
                           PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG,
                           mask,
                           PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_STATUS,
                                      PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_write( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_write", value );
    coreotn_fmf_reg_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_STATUS,
                           PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG,
                           value);
}

static INLINE void coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_action_on_write_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                                     coreotn_handle_t *h_ptr,
                                                                                     UINT32 A,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_action_on_write_field_set( coreotn_fmf_buffer_t *b_ptr,
                                                                                     coreotn_handle_t *h_ptr,
                                                                                     UINT32 A,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_action_on_write_field_set", A, mask, ofs, value );
    coreotn_fmf_action_on_write_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG,
                                           mask,
                                           ofs,
                                           value);

}

static INLINE UINT32 coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_STATUS,
                                      PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                                coreotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                                coreotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return coreotn_fmf_poll( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG,
                             mask,
                             value,
                             cmp,
                             max_count,
                             num_failed_polls,
                             delay_between_polls_in_microseconds);

}


static INLINE UINT32 coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_STATUS,
                                      PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                               coreotn_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                               coreotn_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return coreotn_fmf_poll( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG,
                             mask,
                             value,
                             cmp,
                             max_count,
                             num_failed_polls,
                             delay_between_polls_in_microseconds);

}


static INLINE UINT32 coreotn_fmf_reg_PRBS_TSE_ROLLOVER_STAT_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_PRBS_TSE_ROLLOVER_STAT_REG_read( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_STATUS,
                                      PMC_COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "coreotn_fmf_reg_PRBS_TSE_ROLLOVER_STAT_REG_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 coreotn_fmf_reg_PRBS_TSE_STAT_REG_array_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_reg_PRBS_TSE_STAT_REG_array_read( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = coreotn_fmf_reg_read( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_STATUS,
                                      PMC_COREOTN_FMF_REG_PRBS_TSE_STAT_REG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "coreotn_fmf_reg_PRBS_TSE_STAT_REG_array_read", reg_value, A, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void coreotn_fmf_field_PRBS_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                   coreotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_PRBS_RST_set", A, value );

    /* (0x000ac000 bits 10) field PRBS_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    coreotn_fmf_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_PRBS_RST_MSK,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_PRBS_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_RST_get", A, 1);
    /* (0x000ac000 bits 10) field PRBS_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    reg_value = coreotn_fmf_reg_SW_RESET_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COREOTN_FMF_REG_SW_RESET_REG_BIT_PRBS_RST_MSK) >> COREOTN_FMF_REG_SW_RESET_REG_BIT_PRBS_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_PRBS_RST_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODUKSC2_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODUKSC2_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUKSC2_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODUKSC2_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODUKSC2_RST_set", A, value );

    /* (0x000ac000 bits 9) field ODUKSC2_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    coreotn_fmf_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUKSC2_RST_MSK,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUKSC2_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_fmf_field_ODUKSC2_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODUKSC2_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUKSC2_RST_get", A, 1);
    /* (0x000ac000 bits 9) field ODUKSC2_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    reg_value = coreotn_fmf_reg_SW_RESET_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUKSC2_RST_MSK) >> COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUKSC2_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODUKSC2_RST_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODUJAT_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODUJAT_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUJAT_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODUJAT_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODUJAT_RST_set", A, value );

    /* (0x000ac000 bits 8) field ODUJAT_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    coreotn_fmf_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUJAT_RST_MSK,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUJAT_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_fmf_field_ODUJAT_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODUJAT_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUJAT_RST_get", A, 1);
    /* (0x000ac000 bits 8) field ODUJAT_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    reg_value = coreotn_fmf_reg_SW_RESET_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUJAT_RST_MSK) >> COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUJAT_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODUJAT_RST_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_FRM_RCP96_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_FRM_RCP96_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_FRM_RCP96_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_FRM_RCP96_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_FRM_RCP96_RST_set", A, value );

    /* (0x000ac000 bits 7) field FRM_RCP96_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    coreotn_fmf_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP96_RST_MSK,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP96_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_fmf_field_FRM_RCP96_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_FRM_RCP96_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_FRM_RCP96_RST_get", A, 1);
    /* (0x000ac000 bits 7) field FRM_RCP96_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    reg_value = coreotn_fmf_reg_SW_RESET_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP96_RST_MSK) >> COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP96_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_FRM_RCP96_RST_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_FRM_RCP12_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_FRM_RCP12_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_FRM_RCP12_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_FRM_RCP12_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_FRM_RCP12_RST_set", A, value );

    /* (0x000ac000 bits 6) field FRM_RCP12_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    coreotn_fmf_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP12_RST_MSK,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP12_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_fmf_field_FRM_RCP12_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_FRM_RCP12_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_FRM_RCP12_RST_get", A, 1);
    /* (0x000ac000 bits 6) field FRM_RCP12_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    reg_value = coreotn_fmf_reg_SW_RESET_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP12_RST_MSK) >> COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP12_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_FRM_RCP12_RST_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_RFRM12_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_RFRM12_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM12_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_RFRM12_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_RFRM12_RST_set", A, value );

    /* (0x000ac000 bits 5) field RFRM12_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    coreotn_fmf_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM12_RST_MSK,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM12_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_fmf_field_RFRM12_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_RFRM12_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM12_RST_get", A, 1);
    /* (0x000ac000 bits 5) field RFRM12_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    reg_value = coreotn_fmf_reg_SW_RESET_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM12_RST_MSK) >> COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM12_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_RFRM12_RST_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_TFRM12_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_TFRM12_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM12_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_TFRM12_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_TFRM12_RST_set", A, value );

    /* (0x000ac000 bits 4) field TFRM12_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    coreotn_fmf_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM12_RST_MSK,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM12_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_fmf_field_TFRM12_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_TFRM12_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM12_RST_get", A, 1);
    /* (0x000ac000 bits 4) field TFRM12_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    reg_value = coreotn_fmf_reg_SW_RESET_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM12_RST_MSK) >> COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM12_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_TFRM12_RST_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_RFRM96_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_RFRM96_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM96_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_RFRM96_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_RFRM96_RST_set", A, value );

    /* (0x000ac000 bits 3) field RFRM96_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    coreotn_fmf_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM96_RST_MSK,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM96_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_fmf_field_RFRM96_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_RFRM96_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM96_RST_get", A, 1);
    /* (0x000ac000 bits 3) field RFRM96_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    reg_value = coreotn_fmf_reg_SW_RESET_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM96_RST_MSK) >> COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM96_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_RFRM96_RST_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_TFRM96_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_TFRM96_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                     coreotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM96_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_TFRM96_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_TFRM96_RST_set", A, value );

    /* (0x000ac000 bits 2) field TFRM96_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    coreotn_fmf_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM96_RST_MSK,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM96_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_fmf_field_TFRM96_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_TFRM96_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM96_RST_get", A, 1);
    /* (0x000ac000 bits 2) field TFRM96_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    reg_value = coreotn_fmf_reg_SW_RESET_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM96_RST_MSK) >> COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM96_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_TFRM96_RST_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODTU_DMX_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODTU_DMX_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_DMX_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODTU_DMX_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODTU_DMX_RST_set", A, value );

    /* (0x000ac000 bits 1) field ODTU_DMX_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    coreotn_fmf_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_DMX_RST_MSK,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_DMX_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_fmf_field_ODTU_DMX_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODTU_DMX_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_DMX_RST_get", A, 1);
    /* (0x000ac000 bits 1) field ODTU_DMX_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    reg_value = coreotn_fmf_reg_SW_RESET_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_DMX_RST_MSK) >> COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_DMX_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODTU_DMX_RST_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODTU_MUX_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODTU_MUX_RST_set( coreotn_fmf_buffer_t *b_ptr,
                                                       coreotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_MUX_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODTU_MUX_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODTU_MUX_RST_set", A, value );

    /* (0x000ac000 bits 0) field ODTU_MUX_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    coreotn_fmf_reg_SW_RESET_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_MUX_RST_MSK,
                                            COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_MUX_RST_OFF,
                                            value);
}

static INLINE UINT32 coreotn_fmf_field_ODTU_MUX_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODTU_MUX_RST_get( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_MUX_RST_get", A, 1);
    /* (0x000ac000 bits 0) field ODTU_MUX_RST of register PMC_COREOTN_FMF_REG_SW_RESET_REG */
    reg_value = coreotn_fmf_reg_SW_RESET_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_MUX_RST_MSK) >> COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_MUX_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODTU_MUX_RST_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_FILTER_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_FILTER_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_FILTER_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_FILTER_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_FILTER_LOWPWR_set", A, value );

    /* (0x000ac004 bits 11) field FILTER_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_FILTER_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_FILTER_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_FILTER_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_FILTER_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_FILTER_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 11) field FILTER_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_FILTER_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_FILTER_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_FILTER_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_PRBS_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_PRBS_LOWPWR_set", A, value );

    /* (0x000ac004 bits 10) field PRBS_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_PRBS_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_PRBS_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 10) field PRBS_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_PRBS_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_PRBS_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_PRBS_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODUKSC2_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODUKSC2_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUKSC2_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODUKSC2_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODUKSC2_LOWPWR_set", A, value );

    /* (0x000ac004 bits 9) field ODUKSC2_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUKSC2_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUKSC2_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_ODUKSC2_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODUKSC2_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUKSC2_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 9) field ODUKSC2_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUKSC2_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUKSC2_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODUKSC2_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODUJAT_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODUJAT_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUJAT_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODUJAT_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODUJAT_LOWPWR_set", A, value );

    /* (0x000ac004 bits 8) field ODUJAT_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUJAT_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUJAT_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_ODUJAT_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODUJAT_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUJAT_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 8) field ODUJAT_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUJAT_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUJAT_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODUJAT_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_FRM_RCP96_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_FRM_RCP96_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_FRM_RCP96_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_FRM_RCP96_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_FRM_RCP96_LOWPWR_set", A, value );

    /* (0x000ac004 bits 7) field FRM_RCP96_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP96_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP96_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_FRM_RCP96_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_FRM_RCP96_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_FRM_RCP96_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 7) field FRM_RCP96_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP96_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP96_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_FRM_RCP96_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_FRM_RCP12_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_FRM_RCP12_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_FRM_RCP12_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_FRM_RCP12_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_FRM_RCP12_LOWPWR_set", A, value );

    /* (0x000ac004 bits 6) field FRM_RCP12_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP12_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP12_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_FRM_RCP12_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_FRM_RCP12_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_FRM_RCP12_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 6) field FRM_RCP12_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP12_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP12_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_FRM_RCP12_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_RFRM12_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_RFRM12_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM12_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_RFRM12_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_RFRM12_LOWPWR_set", A, value );

    /* (0x000ac004 bits 5) field RFRM12_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM12_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM12_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_RFRM12_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_RFRM12_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM12_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 5) field RFRM12_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM12_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM12_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_RFRM12_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_TFRM12_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_TFRM12_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM12_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_TFRM12_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_TFRM12_LOWPWR_set", A, value );

    /* (0x000ac004 bits 4) field TFRM12_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM12_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM12_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_TFRM12_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_TFRM12_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM12_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 4) field TFRM12_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM12_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM12_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_TFRM12_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_RFRM96_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_RFRM96_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM96_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_RFRM96_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_RFRM96_LOWPWR_set", A, value );

    /* (0x000ac004 bits 3) field RFRM96_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM96_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM96_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_RFRM96_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_RFRM96_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM96_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 3) field RFRM96_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM96_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM96_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_RFRM96_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_TFRM96_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_TFRM96_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM96_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_TFRM96_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_TFRM96_LOWPWR_set", A, value );

    /* (0x000ac004 bits 2) field TFRM96_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM96_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM96_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_TFRM96_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_TFRM96_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM96_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 2) field TFRM96_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM96_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM96_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_TFRM96_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODTU_DMX_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODTU_DMX_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_DMX_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODTU_DMX_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODTU_DMX_LOWPWR_set", A, value );

    /* (0x000ac004 bits 1) field ODTU_DMX_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_DMX_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_DMX_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_ODTU_DMX_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODTU_DMX_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_DMX_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 1) field ODTU_DMX_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_DMX_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_DMX_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODTU_DMX_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODTU_MUX_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODTU_MUX_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_MUX_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODTU_MUX_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODTU_MUX_LOWPWR_set", A, value );

    /* (0x000ac004 bits 0) field ODTU_MUX_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    coreotn_fmf_reg_CLK_GATING_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_MUX_LOWPWR_MSK,
                                              COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_MUX_LOWPWR_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_ODTU_MUX_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODTU_MUX_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_MUX_LOWPWR_get", A, 1);
    /* (0x000ac004 bits 0) field ODTU_MUX_LOWPWR of register PMC_COREOTN_FMF_REG_CLK_GATING_REG */
    reg_value = coreotn_fmf_reg_CLK_GATING_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_MUX_LOWPWR_MSK) >> COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_MUX_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODTU_MUX_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_PRBS_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_RAM_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_PRBS_RAM_LOWPWR_set", A, value );

    /* (0x000ac008 bits 10) field PRBS_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_PRBS_RAM_LOWPWR_MSK,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_PRBS_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_RAM_LOWPWR_get", A, 1);
    /* (0x000ac008 bits 10) field PRBS_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_fmf_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_PRBS_RAM_LOWPWR_MSK) >> COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_PRBS_RAM_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_PRBS_RAM_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODUKSC2_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODUKSC2_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUKSC2_RAM_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODUKSC2_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODUKSC2_RAM_LOWPWR_set", A, value );

    /* (0x000ac008 bits 9) field ODUKSC2_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUKSC2_RAM_LOWPWR_MSK,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUKSC2_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_fmf_field_ODUKSC2_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODUKSC2_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUKSC2_RAM_LOWPWR_get", A, 1);
    /* (0x000ac008 bits 9) field ODUKSC2_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_fmf_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUKSC2_RAM_LOWPWR_MSK) >> COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUKSC2_RAM_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODUKSC2_RAM_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODUJAT_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODUJAT_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUJAT_RAM_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODUJAT_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODUJAT_RAM_LOWPWR_set", A, value );

    /* (0x000ac008 bits 8) field ODUJAT_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUJAT_RAM_LOWPWR_MSK,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUJAT_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_fmf_field_ODUJAT_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODUJAT_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODUJAT_RAM_LOWPWR_get", A, 1);
    /* (0x000ac008 bits 8) field ODUJAT_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_fmf_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUJAT_RAM_LOWPWR_MSK) >> COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUJAT_RAM_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODUJAT_RAM_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_RFRM12_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_RFRM12_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM12_RAM_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_RFRM12_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_RFRM12_RAM_LOWPWR_set", A, value );

    /* (0x000ac008 bits 5) field RFRM12_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM12_RAM_LOWPWR_MSK,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM12_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_fmf_field_RFRM12_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_RFRM12_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM12_RAM_LOWPWR_get", A, 1);
    /* (0x000ac008 bits 5) field RFRM12_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_fmf_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM12_RAM_LOWPWR_MSK) >> COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM12_RAM_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_RFRM12_RAM_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_TFRM12_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_TFRM12_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM12_RAM_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_TFRM12_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_TFRM12_RAM_LOWPWR_set", A, value );

    /* (0x000ac008 bits 4) field TFRM12_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM12_RAM_LOWPWR_MSK,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM12_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_fmf_field_TFRM12_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_TFRM12_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM12_RAM_LOWPWR_get", A, 1);
    /* (0x000ac008 bits 4) field TFRM12_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_fmf_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM12_RAM_LOWPWR_MSK) >> COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM12_RAM_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_TFRM12_RAM_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_RFRM96_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_RFRM96_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM96_RAM_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_RFRM96_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_RFRM96_RAM_LOWPWR_set", A, value );

    /* (0x000ac008 bits 3) field RFRM96_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM96_RAM_LOWPWR_MSK,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM96_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_fmf_field_RFRM96_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_RFRM96_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_RFRM96_RAM_LOWPWR_get", A, 1);
    /* (0x000ac008 bits 3) field RFRM96_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_fmf_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM96_RAM_LOWPWR_MSK) >> COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM96_RAM_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_RFRM96_RAM_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_TFRM96_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_TFRM96_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM96_RAM_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_TFRM96_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_TFRM96_RAM_LOWPWR_set", A, value );

    /* (0x000ac008 bits 2) field TFRM96_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM96_RAM_LOWPWR_MSK,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM96_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_fmf_field_TFRM96_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_TFRM96_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_TFRM96_RAM_LOWPWR_get", A, 1);
    /* (0x000ac008 bits 2) field TFRM96_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_fmf_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM96_RAM_LOWPWR_MSK) >> COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM96_RAM_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_TFRM96_RAM_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODTU_DMX_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODTU_DMX_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_DMX_RAM_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODTU_DMX_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODTU_DMX_RAM_LOWPWR_set", A, value );

    /* (0x000ac008 bits 1) field ODTU_DMX_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_DMX_RAM_LOWPWR_MSK,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_DMX_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_fmf_field_ODTU_DMX_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODTU_DMX_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_DMX_RAM_LOWPWR_get", A, 1);
    /* (0x000ac008 bits 1) field ODTU_DMX_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_fmf_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_DMX_RAM_LOWPWR_MSK) >> COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_DMX_RAM_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODTU_DMX_RAM_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODTU_MUX_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODTU_MUX_RAM_LOWPWR_set( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_MUX_RAM_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODTU_MUX_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODTU_MUX_RAM_LOWPWR_set", A, value );

    /* (0x000ac008 bits 0) field ODTU_MUX_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_MUX_RAM_LOWPWR_MSK,
                                                 COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_MUX_RAM_LOWPWR_OFF,
                                                 value);
}

static INLINE UINT32 coreotn_fmf_field_ODTU_MUX_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODTU_MUX_RAM_LOWPWR_get( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_MUX_RAM_LOWPWR_get", A, 1);
    /* (0x000ac008 bits 0) field ODTU_MUX_RAM_LOWPWR of register PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG */
    reg_value = coreotn_fmf_reg_RAM_LOW_POWER_REG_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_MUX_RAM_LOWPWR_MSK) >> COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_MUX_RAM_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODTU_MUX_RAM_LOWPWR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_DCI_MUX_CTRL_5_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_DCI_MUX_CTRL_5_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_MUX_CTRL_5_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_5_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_5_set", A, value );

    /* (0x000ac014 bits 16) field DCI_MUX_CTRL_5 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
    coreotn_fmf_reg_DCI_CONFIG_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_5_MSK,
                                              COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_5_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_DCI_MUX_CTRL_5_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_DCI_MUX_CTRL_5_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_MUX_CTRL_5_get", A, 1);
    /* (0x000ac014 bits 16) field DCI_MUX_CTRL_5 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
    reg_value = coreotn_fmf_reg_DCI_CONFIG_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_5_MSK) >> COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_5_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_5_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_DCI_MUX_CTRL_4_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_DCI_MUX_CTRL_4_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_MUX_CTRL_4_set", A, 1);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_4_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_4_set", A, value );

    /* (0x000ac014 bits 13:12) field DCI_MUX_CTRL_4 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
    coreotn_fmf_reg_DCI_CONFIG_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_MSK,
                                              COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_DCI_MUX_CTRL_4_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_DCI_MUX_CTRL_4_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_MUX_CTRL_4_get", A, 1);
    /* (0x000ac014 bits 13:12) field DCI_MUX_CTRL_4 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
    reg_value = coreotn_fmf_reg_DCI_CONFIG_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_MSK) >> COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_4_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_range_DCI_MUX_CTRL_4_set( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_range_DCI_MUX_CTRL_4_set( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_DCI_MUX_CTRL_4_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_DCI_MUX_CTRL_4_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_DCI_MUX_CTRL_4_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_DCI_MUX_CTRL_4_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac014 bits 13:12) field DCI_MUX_CTRL_4 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
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
        /* (0x000ac014 bits 13:12) field DCI_MUX_CTRL_4 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
        coreotn_fmf_reg_DCI_CONFIG_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_OFF + subfield_offset),
                                                  COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_field_range_DCI_MUX_CTRL_4_get( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_DCI_MUX_CTRL_4_get( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_DCI_MUX_CTRL_4_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_DCI_MUX_CTRL_4_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_DCI_MUX_CTRL_4_get", stop_bit, 1 );
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
    /* (0x000ac014 bits 13:12) field DCI_MUX_CTRL_4 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
    reg_value = coreotn_fmf_reg_DCI_CONFIG_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_MSK)
                  >> COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_MSK, COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_DCI_MUX_CTRL_4_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_fmf_field_DCI_MUX_CTRL_3_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_DCI_MUX_CTRL_3_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_MUX_CTRL_3_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_3_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_3_set", A, value );

    /* (0x000ac014 bits 8) field DCI_MUX_CTRL_3 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
    coreotn_fmf_reg_DCI_CONFIG_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_3_MSK,
                                              COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_3_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_DCI_MUX_CTRL_3_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_DCI_MUX_CTRL_3_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_MUX_CTRL_3_get", A, 1);
    /* (0x000ac014 bits 8) field DCI_MUX_CTRL_3 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
    reg_value = coreotn_fmf_reg_DCI_CONFIG_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_3_MSK) >> COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_3_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_DCI_MUX_CTRL_2_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_DCI_MUX_CTRL_2_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_MUX_CTRL_2_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_2_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_2_set", A, value );

    /* (0x000ac014 bits 4) field DCI_MUX_CTRL_2 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
    coreotn_fmf_reg_DCI_CONFIG_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_2_MSK,
                                              COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_2_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_DCI_MUX_CTRL_2_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_DCI_MUX_CTRL_2_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_MUX_CTRL_2_get", A, 1);
    /* (0x000ac014 bits 4) field DCI_MUX_CTRL_2 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
    reg_value = coreotn_fmf_reg_DCI_CONFIG_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_2_MSK) >> COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_2_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_DCI_MUX_CTRL_1_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_DCI_MUX_CTRL_1_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_MUX_CTRL_1_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_1_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_1_set", A, value );

    /* (0x000ac014 bits 0) field DCI_MUX_CTRL_1 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
    coreotn_fmf_reg_DCI_CONFIG_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_1_MSK,
                                              COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_1_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_DCI_MUX_CTRL_1_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_DCI_MUX_CTRL_1_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_MUX_CTRL_1_get", A, 1);
    /* (0x000ac014 bits 0) field DCI_MUX_CTRL_1 of register PMC_COREOTN_FMF_REG_DCI_CONFIG_REG */
    reg_value = coreotn_fmf_reg_DCI_CONFIG_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_1_MSK) >> COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_DCI_MUX_CTRL_1_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_DCI_XOFF_CTRL_3_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_DCI_XOFF_CTRL_3_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_XOFF_CTRL_3_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_DCI_XOFF_CTRL_3_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_DCI_XOFF_CTRL_3_set", A, value );

    /* (0x000ac018 bits 8) field DCI_XOFF_CTRL_3 of register PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG */
    coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_3_MSK,
                                                   COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_3_OFF,
                                                   value);
}

static INLINE UINT32 coreotn_fmf_field_DCI_XOFF_CTRL_3_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_DCI_XOFF_CTRL_3_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_XOFF_CTRL_3_get", A, 1);
    /* (0x000ac018 bits 8) field DCI_XOFF_CTRL_3 of register PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG */
    reg_value = coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_3_MSK) >> COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_DCI_XOFF_CTRL_3_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_DCI_XOFF_CTRL_2_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_DCI_XOFF_CTRL_2_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_XOFF_CTRL_2_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_DCI_XOFF_CTRL_2_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_DCI_XOFF_CTRL_2_set", A, value );

    /* (0x000ac018 bits 4) field DCI_XOFF_CTRL_2 of register PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG */
    coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_2_MSK,
                                                   COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_2_OFF,
                                                   value);
}

static INLINE UINT32 coreotn_fmf_field_DCI_XOFF_CTRL_2_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_DCI_XOFF_CTRL_2_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_XOFF_CTRL_2_get", A, 1);
    /* (0x000ac018 bits 4) field DCI_XOFF_CTRL_2 of register PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG */
    reg_value = coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_2_MSK) >> COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_DCI_XOFF_CTRL_2_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_DCI_XOFF_CTRL_1_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_DCI_XOFF_CTRL_1_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_XOFF_CTRL_1_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_DCI_XOFF_CTRL_1_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_DCI_XOFF_CTRL_1_set", A, value );

    /* (0x000ac018 bits 0) field DCI_XOFF_CTRL_1 of register PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG */
    coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_1_MSK,
                                                   COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_1_OFF,
                                                   value);
}

static INLINE UINT32 coreotn_fmf_field_DCI_XOFF_CTRL_1_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_DCI_XOFF_CTRL_1_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_DCI_XOFF_CTRL_1_get", A, 1);
    /* (0x000ac018 bits 0) field DCI_XOFF_CTRL_1 of register PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG */
    reg_value = coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_1_MSK) >> COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_DCI_XOFF_CTRL_1_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_NORM_RATE_SELECT_set( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_NORM_RATE_SELECT_set( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_NORM_RATE_SELECT_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_NORM_RATE_SELECT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_NORM_RATE_SELECT_set", A, value );

    /* (0x000ac01c bits 8) field NORM_RATE_SELECT of register PMC_COREOTN_FMF_REG_TSB_CONFIG_REG */
    coreotn_fmf_reg_TSB_CONFIG_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_NORM_RATE_SELECT_MSK,
                                              COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_NORM_RATE_SELECT_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_NORM_RATE_SELECT_get( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_NORM_RATE_SELECT_get( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_NORM_RATE_SELECT_get", A, 1);
    /* (0x000ac01c bits 8) field NORM_RATE_SELECT of register PMC_COREOTN_FMF_REG_TSB_CONFIG_REG */
    reg_value = coreotn_fmf_reg_TSB_CONFIG_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_NORM_RATE_SELECT_MSK) >> COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_NORM_RATE_SELECT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_NORM_RATE_SELECT_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_ODTU_MUX_MPMX_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_ODTU_MUX_MPMX_set( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_MUX_MPMX_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_ODTU_MUX_MPMX_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_ODTU_MUX_MPMX_set", A, value );

    /* (0x000ac01c bits 0) field ODTU_MUX_MPMX of register PMC_COREOTN_FMF_REG_TSB_CONFIG_REG */
    coreotn_fmf_reg_TSB_CONFIG_REG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_ODTU_MUX_MPMX_MSK,
                                              COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_ODTU_MUX_MPMX_OFF,
                                              value);
}

static INLINE UINT32 coreotn_fmf_field_ODTU_MUX_MPMX_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_ODTU_MUX_MPMX_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_ODTU_MUX_MPMX_get", A, 1);
    /* (0x000ac01c bits 0) field ODTU_MUX_MPMX of register PMC_COREOTN_FMF_REG_TSB_CONFIG_REG */
    reg_value = coreotn_fmf_reg_TSB_CONFIG_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_ODTU_MUX_MPMX_MSK) >> COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_ODTU_MUX_MPMX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_ODTU_MUX_MPMX_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_REMOTE_RCP12_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_REMOTE_RCP12_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_REMOTE_RCP12_CFG_set", A, 1);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_REMOTE_RCP12_CFG_set", value, 4095);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_REMOTE_RCP12_CFG_set", A, value );

    /* (0x000ac020 bits 11:0) field REMOTE_RCP12_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1 */
    coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_MSK,
                                                       COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_OFF,
                                                       value);
}

static INLINE UINT32 coreotn_fmf_field_REMOTE_RCP12_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_REMOTE_RCP12_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_REMOTE_RCP12_CFG_get", A, 1);
    /* (0x000ac020 bits 11:0) field REMOTE_RCP12_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1 */
    reg_value = coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_MSK) >> COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_REMOTE_RCP12_CFG_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_range_REMOTE_RCP12_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_range_REMOTE_RCP12_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_REMOTE_RCP12_CFG_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_REMOTE_RCP12_CFG_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_REMOTE_RCP12_CFG_set", stop_bit, 11 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_REMOTE_RCP12_CFG_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac020 bits 11:0) field REMOTE_RCP12_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1 */
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
        /* (0x000ac020 bits 11:0) field REMOTE_RCP12_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1 */
        coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_OFF + subfield_offset),
                                                           COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_field_range_REMOTE_RCP12_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_REMOTE_RCP12_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_REMOTE_RCP12_CFG_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_REMOTE_RCP12_CFG_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_REMOTE_RCP12_CFG_get", stop_bit, 11 );
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
    /* (0x000ac020 bits 11:0) field REMOTE_RCP12_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1 */
    reg_value = coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_MSK)
                  >> COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_MSK, COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_REMOTE_RCP12_CFG_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_fmf_field_CFC_FIFO1_CLRB_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_CFC_FIFO1_CLRB_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                             coreotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_CFC_FIFO1_CLRB_CFG_set", A, 1);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_CFC_FIFO1_CLRB_CFG_set", value, 4095);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_CFC_FIFO1_CLRB_CFG_set", A, value );

    /* (0x000ac024 bits 11:0) field CFC_FIFO1_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2 */
    coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_MSK,
                                                       COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_OFF,
                                                       value);
}

static INLINE UINT32 coreotn_fmf_field_CFC_FIFO1_CLRB_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_CFC_FIFO1_CLRB_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_CFC_FIFO1_CLRB_CFG_get", A, 1);
    /* (0x000ac024 bits 11:0) field CFC_FIFO1_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2 */
    reg_value = coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_MSK) >> COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_CFC_FIFO1_CLRB_CFG_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                                   coreotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_set", stop_bit, 11 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac024 bits 11:0) field CFC_FIFO1_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2 */
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
        /* (0x000ac024 bits 11:0) field CFC_FIFO1_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2 */
        coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_OFF + subfield_offset),
                                                           COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_get", stop_bit, 11 );
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
    /* (0x000ac024 bits 11:0) field CFC_FIFO1_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2 */
    reg_value = coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_MSK)
                  >> COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_MSK, COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_fmf_lfield_REMOTE_RCP96_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_lfield_REMOTE_RCP96_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value[3] )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_REMOTE_RCP96_CFG_set", A, 1);
    IOLOG( "%s <= A=%d 0x%08x 0x%08x 0x%08x", "coreotn_fmf_lfield_REMOTE_RCP96_CFG_set", A, value[2] , value[1] , value[0] );

    /* (0x000ac034 bits 31:0) bits 0:31 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1 */
    coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_MSK,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_OFF,
                                                       value[0]);

    /* (0x000ac038 bits 31:0) bits 32:63 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2 */
    coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_MSK,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_OFF,
                                                       value[1]);

    /* (0x000ac03c bits 31:0) bits 64:95 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3 */
    coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_MSK,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_OFF,
                                                       value[2]);
}

static INLINE void coreotn_fmf_lfield_REMOTE_RCP96_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_lfield_REMOTE_RCP96_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_REMOTE_RCP96_CFG_get", A, 1);
    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x000ac034 bits 31:0) bits 0:31 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1 */
    reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_MSK) >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_OFF;
    value[0] |= field_value;

    /* (0x000ac038 bits 31:0) bits 32:63 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2 */
    reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_MSK) >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_OFF;
    value[1] |= field_value;

    /* (0x000ac03c bits 31:0) bits 64:95 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3 */
    reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_MSK) >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> A=%d 0x%08x 0x%08x 0x%08x", "coreotn_fmf_lfield_REMOTE_RCP96_CFG_get", A, value[2] , value[1] , value[0] );

}
static INLINE void coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set", stop_bit, 95 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac034 bits 31:0) bits 0:31 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1 */
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
        /* (0x000ac034 bits 31:0) bits 0:31 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1 */
        coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_OFF + subfield_offset),
                                                           COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac038 bits 31:0) bits 32:63 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2 */
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
        /* (0x000ac038 bits 31:0) bits 32:63 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2 */
        coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_OFF + subfield_offset),
                                                           COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac03c bits 31:0) bits 64:95 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000ac03c bits 31:0) bits 64:95 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3 */
        coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_OFF + subfield_offset),
                                                           COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_get", stop_bit, 95 );
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
    /* (0x000ac034 bits 31:0) bits 0:31 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1 */
        reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_MSK)
                      >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_MSK, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_OFF, field_value );
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
    /* (0x000ac038 bits 31:0) bits 32:63 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2 */
        reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_MSK)
                      >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_MSK, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000ac03c bits 31:0) bits 64:95 use field REMOTE_RCP96_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3 */
        reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_MSK)
                      >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_MSK, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_fmf_lfield_CFC_FIFO2_CLRB_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_lfield_CFC_FIFO2_CLRB_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value[3] )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_CFC_FIFO2_CLRB_CFG_set", A, 1);
    IOLOG( "%s <= A=%d 0x%08x 0x%08x 0x%08x", "coreotn_fmf_lfield_CFC_FIFO2_CLRB_CFG_set", A, value[2] , value[1] , value[0] );

    /* (0x000ac040 bits 31:0) bits 0:31 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4 */
    coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_MSK,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_OFF,
                                                       value[0]);

    /* (0x000ac044 bits 31:0) bits 32:63 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5 */
    coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_MSK,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_OFF,
                                                       value[1]);

    /* (0x000ac048 bits 31:0) bits 64:95 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6 */
    coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_MSK,
                                                       COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_OFF,
                                                       value[2]);
}

static INLINE void coreotn_fmf_lfield_CFC_FIFO2_CLRB_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_lfield_CFC_FIFO2_CLRB_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_CFC_FIFO2_CLRB_CFG_get", A, 1);
    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x000ac040 bits 31:0) bits 0:31 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4 */
    reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_MSK) >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_OFF;
    value[0] |= field_value;

    /* (0x000ac044 bits 31:0) bits 32:63 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5 */
    reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_MSK) >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_OFF;
    value[1] |= field_value;

    /* (0x000ac048 bits 31:0) bits 64:95 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6 */
    reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_MSK) >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> A=%d 0x%08x 0x%08x 0x%08x", "coreotn_fmf_lfield_CFC_FIFO2_CLRB_CFG_get", A, value[2] , value[1] , value[0] );

}
static INLINE void coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_set( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_set", stop_bit, 95 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac040 bits 31:0) bits 0:31 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4 */
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
        /* (0x000ac040 bits 31:0) bits 0:31 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4 */
        coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_OFF + subfield_offset),
                                                           COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac044 bits 31:0) bits 32:63 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5 */
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
        /* (0x000ac044 bits 31:0) bits 32:63 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5 */
        coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_OFF + subfield_offset),
                                                           COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac048 bits 31:0) bits 64:95 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000ac048 bits 31:0) bits 64:95 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6 */
        coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_OFF + subfield_offset),
                                                           COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_get( coreotn_fmf_buffer_t *b_ptr,
                                                                      coreotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_get", stop_bit, 95 );
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
    /* (0x000ac040 bits 31:0) bits 0:31 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4 */
        reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_MSK)
                      >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_MSK, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_OFF, field_value );
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
    /* (0x000ac044 bits 31:0) bits 32:63 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5 */
        reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_MSK)
                      >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_MSK, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000ac048 bits 31:0) bits 64:95 use field CFC_FIFO2_CLRB_CFG of register PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6 */
        reg_value = coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_MSK)
                      >> COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_MSK, COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_fmf_field_SPARE_GPO_set( coreotn_fmf_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_SPARE_GPO_set( coreotn_fmf_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_SPARE_GPO_set", A, 1);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_SPARE_GPO_set", A, value );

    /* (0x000ac094 bits 31:0) field SPARE_GPO of register PMC_COREOTN_FMF_REG_GPO_REG */
    coreotn_fmf_reg_GPO_REG_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_MSK,
                                       COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_OFF,
                                       value);
}

static INLINE UINT32 coreotn_fmf_field_SPARE_GPO_get( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_SPARE_GPO_get( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_SPARE_GPO_get", A, 1);
    /* (0x000ac094 bits 31:0) field SPARE_GPO of register PMC_COREOTN_FMF_REG_GPO_REG */
    reg_value = coreotn_fmf_reg_GPO_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_MSK) >> COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_SPARE_GPO_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_range_SPARE_GPO_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_range_SPARE_GPO_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_SPARE_GPO_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_SPARE_GPO_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_SPARE_GPO_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_SPARE_GPO_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac094 bits 31:0) field SPARE_GPO of register PMC_COREOTN_FMF_REG_GPO_REG */
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
        /* (0x000ac094 bits 31:0) field SPARE_GPO of register PMC_COREOTN_FMF_REG_GPO_REG */
        coreotn_fmf_reg_GPO_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_OFF + subfield_offset),
                                           COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_field_range_SPARE_GPO_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_SPARE_GPO_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_SPARE_GPO_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_SPARE_GPO_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_SPARE_GPO_get", stop_bit, 31 );
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
    /* (0x000ac094 bits 31:0) field SPARE_GPO of register PMC_COREOTN_FMF_REG_GPO_REG */
    reg_value = coreotn_fmf_reg_GPO_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_MSK)
                  >> COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_MSK, COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_SPARE_GPO_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_fmf_lfield_PRBS_BER_THRES_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_lfield_PRBS_BER_THRES_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value[2] )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_PRBS_BER_THRES_set", A, 1);
    IOLOG( "%s <= A=%d 0x%08x 0x%08x", "coreotn_fmf_lfield_PRBS_BER_THRES_set", A, value[1] , value[0] );

    /* (0x000ac12c bits 31:0) bits 0:31 use field PRBS_BER_THRES of register PMC_COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG */
    coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_MSK,
                                                         COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_OFF,
                                                         value[0]);

    /* (0x000ac130 bits 2:0) bits 32:34 use field PRBS_BER_THRES of register PMC_COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG */
    coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_MSK,
                                                         COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_OFF,
                                                         value[1]);
}

static INLINE void coreotn_fmf_lfield_PRBS_BER_THRES_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_lfield_PRBS_BER_THRES_get( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_PRBS_BER_THRES_get", A, 1);
    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x000ac12c bits 31:0) bits 0:31 use field PRBS_BER_THRES of register PMC_COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG */
    reg_value = coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_MSK) >> COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_OFF;
    value[0] |= field_value;

    /* (0x000ac130 bits 2:0) bits 32:34 use field PRBS_BER_THRES of register PMC_COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG */
    reg_value = coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_MSK) >> COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> A=%d 0x%08x 0x%08x", "coreotn_fmf_lfield_PRBS_BER_THRES_get", A, value[1] , value[0] );

}
static INLINE void coreotn_fmf_lfield_range_PRBS_BER_THRES_set( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_lfield_range_PRBS_BER_THRES_set( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_range_PRBS_BER_THRES_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_lfield_range_PRBS_BER_THRES_set", stop_bit, start_bit );
    if (stop_bit > 34) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_lfield_range_PRBS_BER_THRES_set", stop_bit, 34 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_lfield_range_PRBS_BER_THRES_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac12c bits 31:0) bits 0:31 use field PRBS_BER_THRES of register PMC_COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG */
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
        /* (0x000ac12c bits 31:0) bits 0:31 use field PRBS_BER_THRES of register PMC_COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG */
        coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_OFF + subfield_offset),
                                                             COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac130 bits 2:0) bits 32:34 use field PRBS_BER_THRES of register PMC_COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG */
    if ((start_bit <= 34) && (stop_bit >= 32)) {
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
        if (stop_bit < 34) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 34;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000ac130 bits 2:0) bits 32:34 use field PRBS_BER_THRES of register PMC_COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG */
        coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_OFF + subfield_offset),
                                                             COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_lfield_range_PRBS_BER_THRES_get( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_lfield_range_PRBS_BER_THRES_get( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_lfield_range_PRBS_BER_THRES_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_lfield_range_PRBS_BER_THRES_get", stop_bit, start_bit );
    if (stop_bit > 34) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_lfield_range_PRBS_BER_THRES_get", stop_bit, 34 );
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
    /* (0x000ac12c bits 31:0) bits 0:31 use field PRBS_BER_THRES of register PMC_COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG */
        reg_value = coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
        field_value = (reg_value & COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_MSK)
                      >> COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_MSK, COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 34) && (stop_bit >= 32)) {
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
        if (stop_bit < 34) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 34;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000ac130 bits 2:0) bits 32:34 use field PRBS_BER_THRES of register PMC_COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG */
        reg_value = coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
        field_value = (reg_value & COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_MSK)
                      >> COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_MSK, COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_lfield_range_PRBS_BER_THRES_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_fmf_field_PRBS_RESEED_THR_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_RESEED_THR_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_RESEED_THR_set", A, 1);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_RESEED_THR_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_PRBS_RESEED_THR_set", A, value );

    /* (0x000ac134 bits 31:16) field PRBS_RESEED_THR of register PMC_COREOTN_FMF_REG_PRBS_RESEED_REG */
    coreotn_fmf_reg_PRBS_RESEED_REG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_MSK,
                                               COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_OFF,
                                               value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_RESEED_THR_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_RESEED_THR_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_RESEED_THR_get", A, 1);
    /* (0x000ac134 bits 31:16) field PRBS_RESEED_THR of register PMC_COREOTN_FMF_REG_PRBS_RESEED_REG */
    reg_value = coreotn_fmf_reg_PRBS_RESEED_REG_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_MSK) >> COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_PRBS_RESEED_THR_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_range_PRBS_RESEED_THR_set( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_range_PRBS_RESEED_THR_set( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_RESEED_THR_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_RESEED_THR_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_RESEED_THR_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_RESEED_THR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac134 bits 31:16) field PRBS_RESEED_THR of register PMC_COREOTN_FMF_REG_PRBS_RESEED_REG */
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
        /* (0x000ac134 bits 31:16) field PRBS_RESEED_THR of register PMC_COREOTN_FMF_REG_PRBS_RESEED_REG */
        coreotn_fmf_reg_PRBS_RESEED_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_OFF + subfield_offset),
                                                   COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_field_range_PRBS_RESEED_THR_get( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_PRBS_RESEED_THR_get( coreotn_fmf_buffer_t *b_ptr,
                                                                  coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_RESEED_THR_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_RESEED_THR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_RESEED_THR_get", stop_bit, 15 );
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
    /* (0x000ac134 bits 31:16) field PRBS_RESEED_THR of register PMC_COREOTN_FMF_REG_PRBS_RESEED_REG */
    reg_value = coreotn_fmf_reg_PRBS_RESEED_REG_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_MSK)
                  >> COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_MSK, COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_RESEED_THR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_fmf_field_PRBS_RESEED_INTERVAL_set( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_RESEED_INTERVAL_set( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_RESEED_INTERVAL_set", A, 1);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_RESEED_INTERVAL_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_PRBS_RESEED_INTERVAL_set", A, value );

    /* (0x000ac134 bits 15:0) field PRBS_RESEED_INTERVAL of register PMC_COREOTN_FMF_REG_PRBS_RESEED_REG */
    coreotn_fmf_reg_PRBS_RESEED_REG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_MSK,
                                               COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_OFF,
                                               value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_RESEED_INTERVAL_get( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_RESEED_INTERVAL_get( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_RESEED_INTERVAL_get", A, 1);
    /* (0x000ac134 bits 15:0) field PRBS_RESEED_INTERVAL of register PMC_COREOTN_FMF_REG_PRBS_RESEED_REG */
    reg_value = coreotn_fmf_reg_PRBS_RESEED_REG_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_MSK) >> COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_PRBS_RESEED_INTERVAL_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_set( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_set( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac134 bits 15:0) field PRBS_RESEED_INTERVAL of register PMC_COREOTN_FMF_REG_PRBS_RESEED_REG */
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
        /* (0x000ac134 bits 15:0) field PRBS_RESEED_INTERVAL of register PMC_COREOTN_FMF_REG_PRBS_RESEED_REG */
        coreotn_fmf_reg_PRBS_RESEED_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_OFF + subfield_offset),
                                                   COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_get( coreotn_fmf_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_get( coreotn_fmf_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_get", stop_bit, 15 );
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
    /* (0x000ac134 bits 15:0) field PRBS_RESEED_INTERVAL of register PMC_COREOTN_FMF_REG_PRBS_RESEED_REG */
    reg_value = coreotn_fmf_reg_PRBS_RESEED_REG_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_MSK)
                  >> COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_MSK, COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void coreotn_fmf_field_PRBS_CHID_set( coreotn_fmf_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_CHID_set( coreotn_fmf_buffer_t *b_ptr,
                                                    coreotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_CHID_set", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_CHID_set", N, 11);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_CHID_set", value, 127);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_CHID_set", A, N, value );

    /* ((0x000ac09c + (N) * 0x4) bits 14:8) field PRBS_CHID of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
    coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N,
                                                          COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_MSK,
                                                          COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_OFF,
                                                          value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_CHID_get( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_CHID_get( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_CHID_get", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_CHID_get", N, 11);
    /* ((0x000ac09c + (N) * 0x4) bits 14:8) field PRBS_CHID of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
    reg_value = coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_read( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 N);
    value = (reg_value & COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_MSK) >> COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_CHID_get", A, N, value );

    return value;
}
static INLINE void coreotn_fmf_field_range_PRBS_CHID_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_range_PRBS_CHID_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_CHID_set", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_range_PRBS_CHID_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_CHID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_CHID_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_CHID_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000ac09c + (N) * 0x4) bits 14:8) field PRBS_CHID of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
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
        /* ((0x000ac09c + (N) * 0x4) bits 14:8) field PRBS_CHID of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
        coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              N,
                                                              subfield_mask << (COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_OFF + subfield_offset),
                                                              COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_field_range_PRBS_CHID_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_PRBS_CHID_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_CHID_get", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_range_PRBS_CHID_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_CHID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_CHID_get", stop_bit, 6 );
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
    /* ((0x000ac09c + (N) * 0x4) bits 14:8) field PRBS_CHID of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
    reg_value = coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_read( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 N);
    field_value = (reg_value & COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_MSK)
                  >> COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_MSK, COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_CHID_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void coreotn_fmf_field_PRBS_SEL_INV_NONINV_set( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_SEL_INV_NONINV_set( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_SEL_INV_NONINV_set", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_SEL_INV_NONINV_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_SEL_INV_NONINV_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_SEL_INV_NONINV_set", A, N, value );

    /* ((0x000ac09c + (N) * 0x4) bits 3) field PRBS_SEL_INV_NONINV of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
    coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N,
                                                          COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_INV_NONINV_MSK,
                                                          COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_INV_NONINV_OFF,
                                                          value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_SEL_INV_NONINV_get( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_SEL_INV_NONINV_get( coreotn_fmf_buffer_t *b_ptr,
                                                                coreotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_SEL_INV_NONINV_get", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_SEL_INV_NONINV_get", N, 11);
    /* ((0x000ac09c + (N) * 0x4) bits 3) field PRBS_SEL_INV_NONINV of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
    reg_value = coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_read( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 N);
    value = (reg_value & COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_INV_NONINV_MSK) >> COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_INV_NONINV_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_SEL_INV_NONINV_get", A, N, value );

    return value;
}
static INLINE void coreotn_fmf_field_PRBS_SEL_31_11_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_SEL_31_11_set( coreotn_fmf_buffer_t *b_ptr,
                                                         coreotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_SEL_31_11_set", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_SEL_31_11_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_SEL_31_11_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_SEL_31_11_set", A, N, value );

    /* ((0x000ac09c + (N) * 0x4) bits 2) field PRBS_SEL_31_11 of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
    coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N,
                                                          COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_31_11_MSK,
                                                          COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_31_11_OFF,
                                                          value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_SEL_31_11_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_SEL_31_11_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_SEL_31_11_get", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_SEL_31_11_get", N, 11);
    /* ((0x000ac09c + (N) * 0x4) bits 2) field PRBS_SEL_31_11 of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
    reg_value = coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_read( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 N);
    value = (reg_value & COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_31_11_MSK) >> COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_31_11_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_SEL_31_11_get", A, N, value );

    return value;
}
static INLINE void coreotn_fmf_field_PRBS_MON_EN_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_MON_EN_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_MON_EN_set", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_MON_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_MON_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_MON_EN_set", A, N, value );

    /* ((0x000ac09c + (N) * 0x4) bits 1) field PRBS_MON_EN of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
    coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N,
                                                          COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_MON_EN_MSK,
                                                          COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_MON_EN_OFF,
                                                          value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_MON_EN_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_MON_EN_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_MON_EN_get", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_MON_EN_get", N, 11);
    /* ((0x000ac09c + (N) * 0x4) bits 1) field PRBS_MON_EN of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
    reg_value = coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_read( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 N);
    value = (reg_value & COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_MON_EN_MSK) >> COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_MON_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_MON_EN_get", A, N, value );

    return value;
}
static INLINE void coreotn_fmf_field_PRBS_GEN_EN_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_GEN_EN_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_GEN_EN_set", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_GEN_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_GEN_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_GEN_EN_set", A, N, value );

    /* ((0x000ac09c + (N) * 0x4) bits 0) field PRBS_GEN_EN of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
    coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N,
                                                          COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_GEN_EN_MSK,
                                                          COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_GEN_EN_OFF,
                                                          value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_GEN_EN_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_GEN_EN_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_GEN_EN_get", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_GEN_EN_get", N, 11);
    /* ((0x000ac09c + (N) * 0x4) bits 0) field PRBS_GEN_EN of register PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG index N=0..11 */
    reg_value = coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_read( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 N);
    value = (reg_value & COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_GEN_EN_MSK) >> COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_GEN_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_GEN_EN_get", A, N, value );

    return value;
}
static INLINE void coreotn_fmf_field_PRBS_INS_DEFECT_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_INS_DEFECT_set( coreotn_fmf_buffer_t *b_ptr,
                                                          coreotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_INS_DEFECT_set", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_INS_DEFECT_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_INS_DEFECT_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_INS_DEFECT_set", A, N, value );

    /* ((0x000ac0cc + (N) * 0x4) bits 0) field PRBS_INS_DEFECT of register PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG index N=0..11 */
    coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         N,
                                                         COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_BIT_PRBS_INS_DEFECT_MSK,
                                                         COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_BIT_PRBS_INS_DEFECT_OFF,
                                                         value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_INS_DEFECT_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_INS_DEFECT_get( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_INS_DEFECT_get", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_INS_DEFECT_get", N, 11);
    /* ((0x000ac0cc + (N) * 0x4) bits 0) field PRBS_INS_DEFECT of register PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG index N=0..11 */
    reg_value = coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_read( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                N);
    value = (reg_value & COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_BIT_PRBS_INS_DEFECT_MSK) >> COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_BIT_PRBS_INS_DEFECT_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_INS_DEFECT_get", A, N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void coreotn_fmf_field_PRBS_DLSS_E_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_DLSS_E_set( coreotn_fmf_buffer_t *b_ptr,
                                                      coreotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_DLSS_E_set", A, 1);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_DLSS_E_set", value, 4095);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_PRBS_DLSS_E_set", A, value );

    /* (0x000ac144 bits 11:0) field PRBS_DLSS_E of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG */
    coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_MSK,
                                                    COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_OFF,
                                                    value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_DLSS_E_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_DLSS_E_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_DLSS_E_get", A, 1);
    /* (0x000ac144 bits 11:0) field PRBS_DLSS_E of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG */
    reg_value = coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_MSK) >> COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_PRBS_DLSS_E_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_range_PRBS_DLSS_E_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_range_PRBS_DLSS_E_set( coreotn_fmf_buffer_t *b_ptr,
                                                            coreotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_DLSS_E_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_DLSS_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_DLSS_E_set", stop_bit, 11 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_DLSS_E_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac144 bits 11:0) field PRBS_DLSS_E of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG */
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
        /* (0x000ac144 bits 11:0) field PRBS_DLSS_E of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG */
        coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        subfield_mask << (COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_OFF + subfield_offset),
                                                        COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_field_range_PRBS_DLSS_E_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_PRBS_DLSS_E_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_DLSS_E_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_DLSS_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_DLSS_E_get", stop_bit, 11 );
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
    /* (0x000ac144 bits 11:0) field PRBS_DLSS_E of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG */
    reg_value = coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_MSK)
                  >> COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_MSK, COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_DLSS_E_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void coreotn_fmf_field_PRBS_DLSS_I_set_to_clear( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_PRBS_DLSS_I_set_to_clear( coreotn_fmf_buffer_t *b_ptr,
                                                               coreotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_DLSS_I_set_to_clear", A, 1);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "coreotn_fmf_field_PRBS_DLSS_I_set_to_clear", value, 4095);
    IOLOG( "%s <= A=%d 0x%08x", "coreotn_fmf_field_PRBS_DLSS_I_set_to_clear", A, value );

    /* (0x000ac148 bits 11:0) field PRBS_DLSS_I of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG */
    coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_action_on_write_field_set( b_ptr,
                                                                      h_ptr,
                                                                      A,
                                                                      COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_MSK,
                                                                      COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_OFF,
                                                                      value);
}

static INLINE UINT32 coreotn_fmf_field_PRBS_DLSS_I_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_DLSS_I_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_DLSS_I_get", A, 1);
    /* (0x000ac148 bits 11:0) field PRBS_DLSS_I of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG */
    reg_value = coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_MSK) >> COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_PRBS_DLSS_I_get", A, value );

    return value;
}
static INLINE void coreotn_fmf_field_range_PRBS_DLSS_I_set_to_clear( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void coreotn_fmf_field_range_PRBS_DLSS_I_set_to_clear( coreotn_fmf_buffer_t *b_ptr,
                                                                     coreotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_DLSS_I_set_to_clear", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_DLSS_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_DLSS_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_DLSS_I_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac148 bits 11:0) field PRBS_DLSS_I of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG */
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
        /* (0x000ac148 bits 11:0) field PRBS_DLSS_I of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG */
        coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_action_on_write_field_set( b_ptr,
                                                                          h_ptr,
                                                                          A,
                                                                          subfield_mask << (COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_OFF + subfield_offset),
                                                                          COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_OFF + subfield_offset,
                                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 coreotn_fmf_field_range_PRBS_DLSS_I_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_PRBS_DLSS_I_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_DLSS_I_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_DLSS_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_DLSS_I_get", stop_bit, 11 );
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
    /* (0x000ac148 bits 11:0) field PRBS_DLSS_I of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG */
    reg_value = coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_MSK)
                  >> COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_MSK, COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_DLSS_I_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_field_range_PRBS_DLSS_I_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                             coreotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_field_range_PRBS_DLSS_I_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                             coreotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_DLSS_I_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_DLSS_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_DLSS_I_poll", stop_bit, 11 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_DLSS_I_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac148 bits 11:0) field PRBS_DLSS_I of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG */
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
        /* (0x000ac148 bits 11:0) field PRBS_DLSS_I of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG */
        return coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_poll( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            subfield_mask << (COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_OFF + subfield_offset),
                                                            value << (COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_field_PRBS_DLSS_I_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_field_PRBS_DLSS_I_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "coreotn_fmf_field_PRBS_DLSS_I_poll", A, value );

    /* (0x000ac148 bits 11:0) field PRBS_DLSS_I of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG */
    return coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_poll( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_MSK,
                                                        (value<<COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_OFF),
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
static INLINE UINT32 coreotn_fmf_field_PRBS_DLSS_V_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_DLSS_V_get( coreotn_fmf_buffer_t *b_ptr,
                                                        coreotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_DLSS_V_get", A, 1);
    /* (0x000ac14c bits 11:0) field PRBS_DLSS_V of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG */
    reg_value = coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_MSK) >> COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_PRBS_DLSS_V_get", A, value );

    return value;
}
static INLINE UINT32 coreotn_fmf_field_range_PRBS_DLSS_V_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_PRBS_DLSS_V_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_DLSS_V_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_DLSS_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_DLSS_V_get", stop_bit, 11 );
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
    /* (0x000ac14c bits 11:0) field PRBS_DLSS_V of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG */
    reg_value = coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_read( b_ptr,
                                                            h_ptr,
                                                            A);
    field_value = (reg_value & COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_MSK)
                  >> COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_MSK, COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_DLSS_V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_field_range_PRBS_DLSS_V_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                             coreotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_field_range_PRBS_DLSS_V_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                             coreotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_DLSS_V_poll", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_DLSS_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_DLSS_V_poll", stop_bit, 11 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_DLSS_V_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000ac14c bits 11:0) field PRBS_DLSS_V of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG */
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
        /* (0x000ac14c bits 11:0) field PRBS_DLSS_V of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG */
        return coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_poll( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_OFF + subfield_offset),
                                                           value << (COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_OFF + subfield_offset),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_field_PRBS_DLSS_V_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE coreotn_fmf_field_PRBS_DLSS_V_poll( coreotn_fmf_buffer_t *b_ptr,
                                                                       coreotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "coreotn_fmf_field_PRBS_DLSS_V_poll", A, value );

    /* (0x000ac14c bits 11:0) field PRBS_DLSS_V of register PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG */
    return coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_poll( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_MSK,
                                                       (value<<COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_OFF),
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
static INLINE UINT32 coreotn_fmf_field_PRBS_TSE_ROVER_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_TSE_ROVER_get( coreotn_fmf_buffer_t *b_ptr,
                                                           coreotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_TSE_ROVER_get", A, 1);
    /* (0x000ac140 bits 11:0) field PRBS_TSE_ROVER of register PMC_COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG */
    reg_value = coreotn_fmf_reg_PRBS_TSE_ROLLOVER_STAT_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG_BIT_PRBS_TSE_ROVER_MSK) >> COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG_BIT_PRBS_TSE_ROVER_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "coreotn_fmf_field_PRBS_TSE_ROVER_get", A, value );

    return value;
}
static INLINE UINT32 coreotn_fmf_field_range_PRBS_TSE_ROVER_get( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_PRBS_TSE_ROVER_get( coreotn_fmf_buffer_t *b_ptr,
                                                                 coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_TSE_ROVER_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_TSE_ROVER_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_TSE_ROVER_get", stop_bit, 11 );
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
    /* (0x000ac140 bits 11:0) field PRBS_TSE_ROVER of register PMC_COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG */
    reg_value = coreotn_fmf_reg_PRBS_TSE_ROLLOVER_STAT_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    field_value = (reg_value & COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG_BIT_PRBS_TSE_ROVER_MSK)
                  >> COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG_BIT_PRBS_TSE_ROVER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG_BIT_PRBS_TSE_ROVER_MSK, COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG_BIT_PRBS_TSE_ROVER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_TSE_ROVER_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size12
 * ==================================================================================
 */
static INLINE UINT32 coreotn_fmf_field_PRBS_TSE_CHAN_REG_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_PRBS_TSE_CHAN_REG_get( coreotn_fmf_buffer_t *b_ptr,
                                                              coreotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_PRBS_TSE_CHAN_REG_get", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_PRBS_TSE_CHAN_REG_get", N, 11);
    /* ((0x000ac0fc + (N) * 0x4) bits 31:0) field PRBS_TSE_CHAN_REG of register PMC_COREOTN_FMF_REG_PRBS_TSE_STAT_REG index N=0..11 */
    reg_value = coreotn_fmf_reg_PRBS_TSE_STAT_REG_array_read( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              N);
    value = (reg_value & COREOTN_FMF_REG_PRBS_TSE_STAT_REG_BIT_PRBS_TSE_CHAN_REG_MSK) >> COREOTN_FMF_REG_PRBS_TSE_STAT_REG_BIT_PRBS_TSE_CHAN_REG_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "coreotn_fmf_field_PRBS_TSE_CHAN_REG_get", A, N, value );

    return value;
}
static INLINE UINT32 coreotn_fmf_field_range_PRBS_TSE_CHAN_REG_get( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 coreotn_fmf_field_range_PRBS_TSE_CHAN_REG_get( coreotn_fmf_buffer_t *b_ptr,
                                                                    coreotn_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "coreotn_fmf_field_range_PRBS_TSE_CHAN_REG_get", A, 1);
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "coreotn_fmf_field_range_PRBS_TSE_CHAN_REG_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "coreotn_fmf_field_range_PRBS_TSE_CHAN_REG_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "coreotn_fmf_field_range_PRBS_TSE_CHAN_REG_get", stop_bit, 31 );
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
    /* ((0x000ac0fc + (N) * 0x4) bits 31:0) field PRBS_TSE_CHAN_REG of register PMC_COREOTN_FMF_REG_PRBS_TSE_STAT_REG index N=0..11 */
    reg_value = coreotn_fmf_reg_PRBS_TSE_STAT_REG_array_read( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              N);
    field_value = (reg_value & COREOTN_FMF_REG_PRBS_TSE_STAT_REG_BIT_PRBS_TSE_CHAN_REG_MSK)
                  >> COREOTN_FMF_REG_PRBS_TSE_STAT_REG_BIT_PRBS_TSE_CHAN_REG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, COREOTN_FMF_REG_PRBS_TSE_STAT_REG_BIT_PRBS_TSE_CHAN_REG_MSK, COREOTN_FMF_REG_PRBS_TSE_STAT_REG_BIT_PRBS_TSE_CHAN_REG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "coreotn_fmf_field_range_PRBS_TSE_CHAN_REG_get", A, N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _COREOTN_FMF_IO_INLINE_H */
