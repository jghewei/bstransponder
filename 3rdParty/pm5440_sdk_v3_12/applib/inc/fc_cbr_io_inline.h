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
 *     and register accessor functions for the fc_cbr block
 *****************************************************************************/
#ifndef _FC_CBR_IO_INLINE_H
#define _FC_CBR_IO_INLINE_H

#include "fc800_pmg_api.h"
#include "fc_cbr_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FC_CBR_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for fc_cbr
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
    fc800_pmg_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} fc_cbr_buffer_t;
static INLINE void fc_cbr_buffer_init( fc_cbr_buffer_t *b_ptr,
                                       fc800_pmg_handle_t *h_ptr,
                                       UINT32 A ) ALWAYS_INLINE;
static INLINE void fc_cbr_buffer_init( fc_cbr_buffer_t *b_ptr,
                                       fc800_pmg_handle_t *h_ptr,
                                       UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x200) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "fc_cbr_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void fc_cbr_buffer_flush( fc_cbr_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void fc_cbr_buffer_flush( fc_cbr_buffer_t *b_ptr )
{
    IOLOG( "fc_cbr_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 fc_cbr_reg_read( fc_cbr_buffer_t *b_ptr,
                                      fc800_pmg_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_read( fc_cbr_buffer_t *b_ptr,
                                      fc800_pmg_handle_t *h_ptr,
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
static INLINE void fc_cbr_reg_write( fc_cbr_buffer_t *b_ptr,
                                     fc800_pmg_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_write( fc_cbr_buffer_t *b_ptr,
                                     fc800_pmg_handle_t *h_ptr,
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

static INLINE void fc_cbr_field_set( fc_cbr_buffer_t *b_ptr,
                                     fc800_pmg_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_set( fc_cbr_buffer_t *b_ptr,
                                     fc800_pmg_handle_t *h_ptr,
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

static INLINE void fc_cbr_action_on_write_field_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_action_on_write_field_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
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

static INLINE void fc_cbr_burst_read( fc_cbr_buffer_t *b_ptr,
                                      fc800_pmg_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc_cbr_burst_read( fc_cbr_buffer_t *b_ptr,
                                      fc800_pmg_handle_t *h_ptr,
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

static INLINE void fc_cbr_burst_write( fc_cbr_buffer_t *b_ptr,
                                       fc800_pmg_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc_cbr_burst_write( fc_cbr_buffer_t *b_ptr,
                                       fc800_pmg_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE fc_cbr_poll( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 mask,
                                                UINT32 value,
                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                UINT32 max_count,
                                                UINT32 *num_failed_polls,
                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_poll( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
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
 *  register access functions for fc_cbr
 * ==================================================================================
 */

static INLINE void fc_cbr_reg_CFG_write( fc_cbr_buffer_t *b_ptr,
                                         fc800_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_CFG_write( fc_cbr_buffer_t *b_ptr,
                                         fc800_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_reg_CFG_write", value );
    fc_cbr_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_CFG,
                      value);
}

static INLINE void fc_cbr_reg_CFG_field_set( fc_cbr_buffer_t *b_ptr,
                                             fc800_pmg_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_CFG_field_set( fc_cbr_buffer_t *b_ptr,
                                             fc800_pmg_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_reg_CFG_field_set", A, mask, ofs, value );
    fc_cbr_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_CFG,
                      mask,
                      PMC_FC_CBR_REG_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 fc_cbr_reg_CFG_read( fc_cbr_buffer_t *b_ptr,
                                          fc800_pmg_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_CFG_read( fc_cbr_buffer_t *b_ptr,
                                          fc800_pmg_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_FC_CBR_REG_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_reg_RX_CFG_write( fc_cbr_buffer_t *b_ptr,
                                            fc800_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_RX_CFG_write( fc_cbr_buffer_t *b_ptr,
                                            fc800_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_reg_RX_CFG_write", value );
    fc_cbr_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_RX_CFG,
                      value);
}

static INLINE void fc_cbr_reg_RX_CFG_field_set( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_RX_CFG_field_set( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_reg_RX_CFG_field_set", A, mask, ofs, value );
    fc_cbr_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_RX_CFG,
                      mask,
                      PMC_FC_CBR_REG_RX_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 fc_cbr_reg_RX_CFG_read( fc_cbr_buffer_t *b_ptr,
                                             fc800_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_RX_CFG_read( fc_cbr_buffer_t *b_ptr,
                                             fc800_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_FC_CBR_REG_RX_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_RX_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_reg_RX_DLOLB_CLK_CFG_write( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_RX_DLOLB_CLK_CFG_write( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_reg_RX_DLOLB_CLK_CFG_write", value );
    fc_cbr_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_RX_DLOLB_CLK_CFG,
                      value);
}

static INLINE void fc_cbr_reg_RX_DLOLB_CLK_CFG_field_set( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_RX_DLOLB_CLK_CFG_field_set( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_reg_RX_DLOLB_CLK_CFG_field_set", A, mask, ofs, value );
    fc_cbr_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_RX_DLOLB_CLK_CFG,
                      mask,
                      PMC_FC_CBR_REG_RX_DLOLB_CLK_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 fc_cbr_reg_RX_DLOLB_CLK_CFG_read( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_RX_DLOLB_CLK_CFG_read( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_FC_CBR_REG_RX_DLOLB_CLK_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_RX_DLOLB_CLK_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_reg_RX_DLOLB_EN_write( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_RX_DLOLB_EN_write( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_reg_RX_DLOLB_EN_write", value );
    fc_cbr_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_RX_DLOLB_EN,
                      value);
}

static INLINE void fc_cbr_reg_RX_DLOLB_EN_field_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_RX_DLOLB_EN_field_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_reg_RX_DLOLB_EN_field_set", A, mask, ofs, value );
    fc_cbr_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_RX_DLOLB_EN,
                      mask,
                      PMC_FC_CBR_REG_RX_DLOLB_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 fc_cbr_reg_RX_DLOLB_EN_read( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_RX_DLOLB_EN_read( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_FC_CBR_REG_RX_DLOLB_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_RX_DLOLB_EN_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_reg_RX_DEFECT_CFG_write( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_RX_DEFECT_CFG_write( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_reg_RX_DEFECT_CFG_write", value );
    fc_cbr_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_RX_DEFECT_CFG,
                      value);
}

static INLINE void fc_cbr_reg_RX_DEFECT_CFG_field_set( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_RX_DEFECT_CFG_field_set( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_reg_RX_DEFECT_CFG_field_set", A, mask, ofs, value );
    fc_cbr_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_RX_DEFECT_CFG,
                      mask,
                      PMC_FC_CBR_REG_RX_DEFECT_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 fc_cbr_reg_RX_DEFECT_CFG_read( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_RX_DEFECT_CFG_read( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_FC_CBR_REG_RX_DEFECT_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_RX_DEFECT_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_reg_TX_USER_PATTERN_LSB_write( fc_cbr_buffer_t *b_ptr,
                                                         fc800_pmg_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_TX_USER_PATTERN_LSB_write( fc_cbr_buffer_t *b_ptr,
                                                         fc800_pmg_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_reg_TX_USER_PATTERN_LSB_write", value );
    fc_cbr_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_TX_USER_PATTERN_LSB,
                      value);
}

static INLINE void fc_cbr_reg_TX_USER_PATTERN_LSB_field_set( fc_cbr_buffer_t *b_ptr,
                                                             fc800_pmg_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_TX_USER_PATTERN_LSB_field_set( fc_cbr_buffer_t *b_ptr,
                                                             fc800_pmg_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_reg_TX_USER_PATTERN_LSB_field_set", A, mask, ofs, value );
    fc_cbr_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_TX_USER_PATTERN_LSB,
                      mask,
                      PMC_FC_CBR_REG_TX_USER_PATTERN_LSB_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 fc_cbr_reg_TX_USER_PATTERN_LSB_read( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_TX_USER_PATTERN_LSB_read( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_FC_CBR_REG_TX_USER_PATTERN_LSB);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_TX_USER_PATTERN_LSB_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_reg_TX_CFG_write( fc_cbr_buffer_t *b_ptr,
                                            fc800_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_TX_CFG_write( fc_cbr_buffer_t *b_ptr,
                                            fc800_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_reg_TX_CFG_write", value );
    fc_cbr_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_TX_CFG,
                      value);
}

static INLINE void fc_cbr_reg_TX_CFG_field_set( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_TX_CFG_field_set( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_reg_TX_CFG_field_set", A, mask, ofs, value );
    fc_cbr_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_FC_CBR_REG_TX_CFG,
                      mask,
                      PMC_FC_CBR_REG_TX_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 fc_cbr_reg_TX_CFG_read( fc_cbr_buffer_t *b_ptr,
                                             fc800_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_TX_CFG_read( fc_cbr_buffer_t *b_ptr,
                                             fc800_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_FC_CBR_REG_TX_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_TX_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_reg_CNT_UPDATE_write( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_CNT_UPDATE_write( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_reg_CNT_UPDATE_write", value );
    fc_cbr_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_FC_CBR_REG_CNT_UPDATE,
                      value);
}

static INLINE void fc_cbr_reg_CNT_UPDATE_field_set( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_CNT_UPDATE_field_set( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_reg_CNT_UPDATE_field_set", A, mask, ofs, value );
    fc_cbr_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_FC_CBR_REG_CNT_UPDATE,
                      mask,
                      PMC_FC_CBR_REG_CNT_UPDATE_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 fc_cbr_reg_CNT_UPDATE_read( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_CNT_UPDATE_read( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_REG_CNT_UPDATE);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_CNT_UPDATE_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_reg_INT_write( fc_cbr_buffer_t *b_ptr,
                                         fc800_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_INT_write( fc_cbr_buffer_t *b_ptr,
                                         fc800_pmg_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_reg_INT_write", value );
    fc_cbr_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_FC_CBR_REG_INT,
                      value);
}

static INLINE void fc_cbr_reg_INT_action_on_write_field_set( fc_cbr_buffer_t *b_ptr,
                                                             fc800_pmg_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_INT_action_on_write_field_set( fc_cbr_buffer_t *b_ptr,
                                                             fc800_pmg_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_reg_INT_action_on_write_field_set", A, mask, ofs, value );
    fc_cbr_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_STATUS,
                                      PMC_FC_CBR_REG_INT,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 fc_cbr_reg_INT_read( fc_cbr_buffer_t *b_ptr,
                                          fc800_pmg_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_INT_read( fc_cbr_buffer_t *b_ptr,
                                          fc800_pmg_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_REG_INT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_INT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_reg_INT_poll( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 value,
                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                        UINT32 max_count,
                                                        UINT32 *num_failed_polls,
                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_reg_INT_poll( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 value,
                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                        UINT32 max_count,
                                                        UINT32 *num_failed_polls,
                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc_cbr_reg_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc_cbr_poll( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_STATUS,
                        PMC_FC_CBR_REG_INT,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void fc_cbr_reg_INT_EN_write( fc_cbr_buffer_t *b_ptr,
                                            fc800_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_INT_EN_write( fc_cbr_buffer_t *b_ptr,
                                            fc800_pmg_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_reg_INT_EN_write", value );
    fc_cbr_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_FC_CBR_REG_INT_EN,
                      value);
}

static INLINE void fc_cbr_reg_INT_EN_field_set( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_reg_INT_EN_field_set( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_reg_INT_EN_field_set", A, mask, ofs, value );
    fc_cbr_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_FC_CBR_REG_INT_EN,
                      mask,
                      PMC_FC_CBR_REG_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 fc_cbr_reg_INT_EN_read( fc_cbr_buffer_t *b_ptr,
                                             fc800_pmg_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_INT_EN_read( fc_cbr_buffer_t *b_ptr,
                                             fc800_pmg_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_REG_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc_cbr_reg_INT_VAL_read( fc_cbr_buffer_t *b_ptr,
                                              fc800_pmg_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_INT_VAL_read( fc_cbr_buffer_t *b_ptr,
                                              fc800_pmg_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_REG_INT_VAL);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_INT_VAL_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_reg_INT_VAL_poll( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_reg_INT_VAL_poll( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc_cbr_reg_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc_cbr_poll( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_STATUS,
                        PMC_FC_CBR_REG_INT_VAL,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 fc_cbr_reg_RX_GF_WRD_CNTR_read( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_RX_GF_WRD_CNTR_read( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_REG_RX_GF_WRD_CNTR);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_RX_GF_WRD_CNTR_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc_cbr_reg_RX_BF_WRD_CNTR_read( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_RX_BF_WRD_CNTR_read( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_REG_RX_BF_WRD_CNTR);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_RX_BF_WRD_CNTR_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc_cbr_reg_RX_WORD_CNT_read( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_RX_WORD_CNT_read( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_REG_RX_WORD_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_RX_WORD_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc_cbr_reg_RX_FRAMES_BAD_CRC_read( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_RX_FRAMES_BAD_CRC_read( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_REG_RX_FRAMES_BAD_CRC);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_RX_FRAMES_BAD_CRC_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc_cbr_reg_RX_TOTAL_PACKETS_read( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_RX_TOTAL_PACKETS_read( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_REG_RX_TOTAL_PACKETS);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_RX_TOTAL_PACKETS_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc_cbr_reg_LCV_CNT_read( fc_cbr_buffer_t *b_ptr,
                                              fc800_pmg_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_reg_LCV_CNT_read( fc_cbr_buffer_t *b_ptr,
                                              fc800_pmg_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_REG_LCV_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_reg_LCV_CNT_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void fc_cbr_field_RX_FRM_SCRAM_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_FRM_SCRAM_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_FRM_SCRAM_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_FRM_SCRAM_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_FRM_SCRAM_set", A, value );

    /* (0x00000000 bits 31) field RX_FRM_SCRAM of register PMC_FC_CBR_REG_CFG */
    fc_cbr_reg_CFG_field_set( b_ptr,
                              h_ptr,
                              A,
                              FC_CBR_REG_CFG_BIT_RX_FRM_SCRAM_MSK,
                              FC_CBR_REG_CFG_BIT_RX_FRM_SCRAM_OFF,
                              value);
}

static INLINE UINT32 fc_cbr_field_RX_FRM_SCRAM_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_FRM_SCRAM_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_FRM_SCRAM_get", A, 12);
    /* (0x00000000 bits 31) field RX_FRM_SCRAM of register PMC_FC_CBR_REG_CFG */
    reg_value = fc_cbr_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_CFG_BIT_RX_FRM_SCRAM_MSK) >> FC_CBR_REG_CFG_BIT_RX_FRM_SCRAM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_FRM_SCRAM_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_BUS_WIDTH_set( fc_cbr_buffer_t *b_ptr,
                                               fc800_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_BUS_WIDTH_set( fc_cbr_buffer_t *b_ptr,
                                               fc800_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_BUS_WIDTH_set", A, 12);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_BUS_WIDTH_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_BUS_WIDTH_set", A, value );

    /* (0x00000000 bits 5:3) bits 0:2 use field BUS_WIDTH of register PMC_FC_CBR_REG_CFG */
    fc_cbr_reg_CFG_field_set( b_ptr,
                              h_ptr,
                              A,
                              FC_CBR_REG_CFG_BIT_BUS_WIDTH_MSK,
                              FC_CBR_REG_CFG_BIT_BUS_WIDTH_OFF,
                              value);
}

static INLINE UINT32 fc_cbr_field_BUS_WIDTH_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_BUS_WIDTH_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_BUS_WIDTH_get", A, 12);
    /* (0x00000000 bits 5:3) bits 0:2 use field BUS_WIDTH of register PMC_FC_CBR_REG_CFG */
    reg_value = fc_cbr_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_CFG_BIT_BUS_WIDTH_MSK) >> FC_CBR_REG_CFG_BIT_BUS_WIDTH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_BUS_WIDTH_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_range_BUS_WIDTH_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_range_BUS_WIDTH_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_BUS_WIDTH_set", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_BUS_WIDTH_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_BUS_WIDTH_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_BUS_WIDTH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 5:3) bits 0:2 use field BUS_WIDTH of register PMC_FC_CBR_REG_CFG */
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
        /* (0x00000000 bits 5:3) bits 0:2 use field BUS_WIDTH of register PMC_FC_CBR_REG_CFG */
        fc_cbr_reg_CFG_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  subfield_mask << (FC_CBR_REG_CFG_BIT_BUS_WIDTH_OFF + subfield_offset),
                                  FC_CBR_REG_CFG_BIT_BUS_WIDTH_OFF + subfield_offset,
                                  value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_field_range_BUS_WIDTH_get( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_BUS_WIDTH_get( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_BUS_WIDTH_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_BUS_WIDTH_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_BUS_WIDTH_get", stop_bit, 2 );
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
    /* (0x00000000 bits 5:3) bits 0:2 use field BUS_WIDTH of register PMC_FC_CBR_REG_CFG */
    reg_value = fc_cbr_reg_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_CFG_BIT_BUS_WIDTH_MSK)
                  >> FC_CBR_REG_CFG_BIT_BUS_WIDTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_CFG_BIT_BUS_WIDTH_MSK, FC_CBR_REG_CFG_BIT_BUS_WIDTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_BUS_WIDTH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_field_MODE_set( fc_cbr_buffer_t *b_ptr,
                                          fc800_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_MODE_set( fc_cbr_buffer_t *b_ptr,
                                          fc800_pmg_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_MODE_set", A, 12);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_MODE_set", A, value );

    /* (0x00000000 bits 2:1) bits 0:1 use field MODE of register PMC_FC_CBR_REG_CFG */
    fc_cbr_reg_CFG_field_set( b_ptr,
                              h_ptr,
                              A,
                              FC_CBR_REG_CFG_BIT_MODE_MSK,
                              FC_CBR_REG_CFG_BIT_MODE_OFF,
                              value);
}

static INLINE UINT32 fc_cbr_field_MODE_get( fc_cbr_buffer_t *b_ptr,
                                            fc800_pmg_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_MODE_get( fc_cbr_buffer_t *b_ptr,
                                            fc800_pmg_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_MODE_get", A, 12);
    /* (0x00000000 bits 2:1) bits 0:1 use field MODE of register PMC_FC_CBR_REG_CFG */
    reg_value = fc_cbr_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_CFG_BIT_MODE_MSK) >> FC_CBR_REG_CFG_BIT_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_MODE_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_range_MODE_set( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_range_MODE_set( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_MODE_set", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 2:1) bits 0:1 use field MODE of register PMC_FC_CBR_REG_CFG */
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
        /* (0x00000000 bits 2:1) bits 0:1 use field MODE of register PMC_FC_CBR_REG_CFG */
        fc_cbr_reg_CFG_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  subfield_mask << (FC_CBR_REG_CFG_BIT_MODE_OFF + subfield_offset),
                                  FC_CBR_REG_CFG_BIT_MODE_OFF + subfield_offset,
                                  value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_field_range_MODE_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_MODE_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_MODE_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_MODE_get", stop_bit, 1 );
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
    /* (0x00000000 bits 2:1) bits 0:1 use field MODE of register PMC_FC_CBR_REG_CFG */
    reg_value = fc_cbr_reg_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_CFG_BIT_MODE_MSK)
                  >> FC_CBR_REG_CFG_BIT_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_CFG_BIT_MODE_MSK, FC_CBR_REG_CFG_BIT_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_field_SOFT_RST_set( fc_cbr_buffer_t *b_ptr,
                                              fc800_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_SOFT_RST_set( fc_cbr_buffer_t *b_ptr,
                                              fc800_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_SOFT_RST_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_SOFT_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_SOFT_RST_set", A, value );

    /* (0x00000000 bits 0) field SOFT_RST of register PMC_FC_CBR_REG_CFG */
    fc_cbr_reg_CFG_field_set( b_ptr,
                              h_ptr,
                              A,
                              FC_CBR_REG_CFG_BIT_SOFT_RST_MSK,
                              FC_CBR_REG_CFG_BIT_SOFT_RST_OFF,
                              value);
}

static INLINE UINT32 fc_cbr_field_SOFT_RST_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_SOFT_RST_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_SOFT_RST_get", A, 12);
    /* (0x00000000 bits 0) field SOFT_RST of register PMC_FC_CBR_REG_CFG */
    reg_value = fc_cbr_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_CFG_BIT_SOFT_RST_MSK) >> FC_CBR_REG_CFG_BIT_SOFT_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_SOFT_RST_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_FC_CONT_ALIGN_MODE_set( fc_cbr_buffer_t *b_ptr,
                                                           fc800_pmg_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_FC_CONT_ALIGN_MODE_set( fc_cbr_buffer_t *b_ptr,
                                                           fc800_pmg_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_FC_CONT_ALIGN_MODE_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_FC_CONT_ALIGN_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_FC_CONT_ALIGN_MODE_set", A, value );

    /* (0x00000014 bits 8) field RX_FC_CONT_ALIGN_MODE of register PMC_FC_CBR_REG_RX_CFG */
    fc_cbr_reg_RX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_RX_CFG_BIT_RX_FC_CONT_ALIGN_MODE_MSK,
                                 FC_CBR_REG_RX_CFG_BIT_RX_FC_CONT_ALIGN_MODE_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_RX_FC_CONT_ALIGN_MODE_get( fc_cbr_buffer_t *b_ptr,
                                                             fc800_pmg_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_FC_CONT_ALIGN_MODE_get( fc_cbr_buffer_t *b_ptr,
                                                             fc800_pmg_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_FC_CONT_ALIGN_MODE_get", A, 12);
    /* (0x00000014 bits 8) field RX_FC_CONT_ALIGN_MODE of register PMC_FC_CBR_REG_RX_CFG */
    reg_value = fc_cbr_reg_RX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_CFG_BIT_RX_FC_CONT_ALIGN_MODE_MSK) >> FC_CBR_REG_RX_CFG_BIT_RX_FC_CONT_ALIGN_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_FC_CONT_ALIGN_MODE_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_ENABLE_BYTE_ALIGN_set( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_ENABLE_BYTE_ALIGN_set( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_ENABLE_BYTE_ALIGN_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_ENABLE_BYTE_ALIGN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_ENABLE_BYTE_ALIGN_set", A, value );

    /* (0x00000014 bits 2) field RX_ENABLE_BYTE_ALIGN of register PMC_FC_CBR_REG_RX_CFG */
    fc_cbr_reg_RX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_RX_CFG_BIT_RX_ENABLE_BYTE_ALIGN_MSK,
                                 FC_CBR_REG_RX_CFG_BIT_RX_ENABLE_BYTE_ALIGN_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_RX_ENABLE_BYTE_ALIGN_get( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_ENABLE_BYTE_ALIGN_get( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_ENABLE_BYTE_ALIGN_get", A, 12);
    /* (0x00000014 bits 2) field RX_ENABLE_BYTE_ALIGN of register PMC_FC_CBR_REG_RX_CFG */
    reg_value = fc_cbr_reg_RX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_CFG_BIT_RX_ENABLE_BYTE_ALIGN_MSK) >> FC_CBR_REG_RX_CFG_BIT_RX_ENABLE_BYTE_ALIGN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_ENABLE_BYTE_ALIGN_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_SOFT_RST_set( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_SOFT_RST_set( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_SOFT_RST_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_SOFT_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_SOFT_RST_set", A, value );

    /* (0x00000014 bits 0) field RX_SOFT_RST of register PMC_FC_CBR_REG_RX_CFG */
    fc_cbr_reg_RX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_RX_CFG_BIT_RX_SOFT_RST_MSK,
                                 FC_CBR_REG_RX_CFG_BIT_RX_SOFT_RST_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_RX_SOFT_RST_get( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_SOFT_RST_get( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_SOFT_RST_get", A, 12);
    /* (0x00000014 bits 0) field RX_SOFT_RST of register PMC_FC_CBR_REG_RX_CFG */
    reg_value = fc_cbr_reg_RX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_CFG_BIT_RX_SOFT_RST_MSK) >> FC_CBR_REG_RX_CFG_BIT_RX_SOFT_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_SOFT_RST_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_DLOLB_CLK_DIV_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_DLOLB_CLK_DIV_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DLOLB_CLK_DIV_set", A, 12);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_DLOLB_CLK_DIV_set", A, value );

    /* (0x0000001c bits 31:0) bits 0:31 use field RX_DLOLB_CLK_DIV of register PMC_FC_CBR_REG_RX_DLOLB_CLK_CFG */
    fc_cbr_reg_RX_DLOLB_CLK_CFG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_MSK,
                                           FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_OFF,
                                           value);
}

static INLINE UINT32 fc_cbr_field_RX_DLOLB_CLK_DIV_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_DLOLB_CLK_DIV_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DLOLB_CLK_DIV_get", A, 12);
    /* (0x0000001c bits 31:0) bits 0:31 use field RX_DLOLB_CLK_DIV of register PMC_FC_CBR_REG_RX_DLOLB_CLK_CFG */
    reg_value = fc_cbr_reg_RX_DLOLB_CLK_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_MSK) >> FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_DLOLB_CLK_DIV_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_range_RX_DLOLB_CLK_DIV_set( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_range_RX_DLOLB_CLK_DIV_set( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_DLOLB_CLK_DIV_set", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_DLOLB_CLK_DIV_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_DLOLB_CLK_DIV_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_DLOLB_CLK_DIV_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000001c bits 31:0) bits 0:31 use field RX_DLOLB_CLK_DIV of register PMC_FC_CBR_REG_RX_DLOLB_CLK_CFG */
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
        /* (0x0000001c bits 31:0) bits 0:31 use field RX_DLOLB_CLK_DIV of register PMC_FC_CBR_REG_RX_DLOLB_CLK_CFG */
        fc_cbr_reg_RX_DLOLB_CLK_CFG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_OFF + subfield_offset),
                                               FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_field_range_RX_DLOLB_CLK_DIV_get( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_RX_DLOLB_CLK_DIV_get( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_DLOLB_CLK_DIV_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_DLOLB_CLK_DIV_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_DLOLB_CLK_DIV_get", stop_bit, 31 );
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
    /* (0x0000001c bits 31:0) bits 0:31 use field RX_DLOLB_CLK_DIV of register PMC_FC_CBR_REG_RX_DLOLB_CLK_CFG */
    reg_value = fc_cbr_reg_RX_DLOLB_CLK_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_MSK)
                  >> FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_MSK, FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_DLOLB_CLK_DIV_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_field_RX_LCV_DLOLB_EN_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_LCV_DLOLB_EN_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LCV_DLOLB_EN_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_LCV_DLOLB_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_LCV_DLOLB_EN_set", A, value );

    /* (0x00000020 bits 7) field RX_LCV_DLOLB_EN of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    fc_cbr_reg_RX_DLOLB_EN_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LCV_DLOLB_EN_MSK,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LCV_DLOLB_EN_OFF,
                                      value);
}

static INLINE UINT32 fc_cbr_field_RX_LCV_DLOLB_EN_get( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_LCV_DLOLB_EN_get( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LCV_DLOLB_EN_get", A, 12);
    /* (0x00000020 bits 7) field RX_LCV_DLOLB_EN of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    reg_value = fc_cbr_reg_RX_DLOLB_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LCV_DLOLB_EN_MSK) >> FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LCV_DLOLB_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_LCV_DLOLB_EN_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_XDET_DLOLB_EN_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_XDET_DLOLB_EN_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_XDET_DLOLB_EN_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_XDET_DLOLB_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_XDET_DLOLB_EN_set", A, value );

    /* (0x00000020 bits 6) field RX_XDET_DLOLB_EN of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    fc_cbr_reg_RX_DLOLB_EN_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_RX_XDET_DLOLB_EN_MSK,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_RX_XDET_DLOLB_EN_OFF,
                                      value);
}

static INLINE UINT32 fc_cbr_field_RX_XDET_DLOLB_EN_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_XDET_DLOLB_EN_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_XDET_DLOLB_EN_get", A, 12);
    /* (0x00000020 bits 6) field RX_XDET_DLOLB_EN of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    reg_value = fc_cbr_reg_RX_DLOLB_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_DLOLB_EN_BIT_RX_XDET_DLOLB_EN_MSK) >> FC_CBR_REG_RX_DLOLB_EN_BIT_RX_XDET_DLOLB_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_XDET_DLOLB_EN_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_ASD_DLOLB_EN_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_ASD_DLOLB_EN_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_ASD_DLOLB_EN_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_ASD_DLOLB_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_ASD_DLOLB_EN_set", A, value );

    /* (0x00000020 bits 5) field ASD_DLOLB_EN of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    fc_cbr_reg_RX_DLOLB_EN_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_ASD_DLOLB_EN_MSK,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_ASD_DLOLB_EN_OFF,
                                      value);
}

static INLINE UINT32 fc_cbr_field_ASD_DLOLB_EN_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_ASD_DLOLB_EN_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_ASD_DLOLB_EN_get", A, 12);
    /* (0x00000020 bits 5) field ASD_DLOLB_EN of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    reg_value = fc_cbr_reg_RX_DLOLB_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_DLOLB_EN_BIT_ASD_DLOLB_EN_MSK) >> FC_CBR_REG_RX_DLOLB_EN_BIT_ASD_DLOLB_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_ASD_DLOLB_EN_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_set( fc_cbr_buffer_t *b_ptr,
                                                           fc800_pmg_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_set( fc_cbr_buffer_t *b_ptr,
                                                           fc800_pmg_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_set", A, value );

    /* (0x00000020 bits 4) field RX_LOSS_SYNC_DLOLB_EN of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    fc_cbr_reg_RX_DLOLB_EN_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LOSS_SYNC_DLOLB_EN_MSK,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LOSS_SYNC_DLOLB_EN_OFF,
                                      value);
}

static INLINE UINT32 fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_get( fc_cbr_buffer_t *b_ptr,
                                                             fc800_pmg_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_get( fc_cbr_buffer_t *b_ptr,
                                                             fc800_pmg_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_get", A, 12);
    /* (0x00000020 bits 4) field RX_LOSS_SYNC_DLOLB_EN of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    reg_value = fc_cbr_reg_RX_DLOLB_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LOSS_SYNC_DLOLB_EN_MSK) >> FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LOSS_SYNC_DLOLB_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_DRL_DLOLB_EN_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_DRL_DLOLB_EN_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DRL_DLOLB_EN_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_DRL_DLOLB_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_DRL_DLOLB_EN_set", A, value );

    /* (0x00000020 bits 3) field RX_DRL_DLOLB_EN of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    fc_cbr_reg_RX_DLOLB_EN_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DRL_DLOLB_EN_MSK,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DRL_DLOLB_EN_OFF,
                                      value);
}

static INLINE UINT32 fc_cbr_field_RX_DRL_DLOLB_EN_get( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_DRL_DLOLB_EN_get( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DRL_DLOLB_EN_get", A, 12);
    /* (0x00000020 bits 3) field RX_DRL_DLOLB_EN of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    reg_value = fc_cbr_reg_RX_DLOLB_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DRL_DLOLB_EN_MSK) >> FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DRL_DLOLB_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_DRL_DLOLB_EN_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_DLOLB_INT_PER_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_DLOLB_INT_PER_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DLOLB_INT_PER_set", A, 12);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_DLOLB_INT_PER_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_DLOLB_INT_PER_set", A, value );

    /* (0x00000020 bits 2:0) bits 0:2 use field RX_DLOLB_INT_PER of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    fc_cbr_reg_RX_DLOLB_EN_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_MSK,
                                      FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_OFF,
                                      value);
}

static INLINE UINT32 fc_cbr_field_RX_DLOLB_INT_PER_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_DLOLB_INT_PER_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DLOLB_INT_PER_get", A, 12);
    /* (0x00000020 bits 2:0) bits 0:2 use field RX_DLOLB_INT_PER of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    reg_value = fc_cbr_reg_RX_DLOLB_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_MSK) >> FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_DLOLB_INT_PER_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_range_RX_DLOLB_INT_PER_set( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_range_RX_DLOLB_INT_PER_set( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_DLOLB_INT_PER_set", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_DLOLB_INT_PER_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_DLOLB_INT_PER_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_DLOLB_INT_PER_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000020 bits 2:0) bits 0:2 use field RX_DLOLB_INT_PER of register PMC_FC_CBR_REG_RX_DLOLB_EN */
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
        /* (0x00000020 bits 2:0) bits 0:2 use field RX_DLOLB_INT_PER of register PMC_FC_CBR_REG_RX_DLOLB_EN */
        fc_cbr_reg_RX_DLOLB_EN_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_OFF + subfield_offset),
                                          FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_field_range_RX_DLOLB_INT_PER_get( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_RX_DLOLB_INT_PER_get( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_DLOLB_INT_PER_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_DLOLB_INT_PER_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_DLOLB_INT_PER_get", stop_bit, 2 );
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
    /* (0x00000020 bits 2:0) bits 0:2 use field RX_DLOLB_INT_PER of register PMC_FC_CBR_REG_RX_DLOLB_EN */
    reg_value = fc_cbr_reg_RX_DLOLB_EN_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_MSK)
                  >> FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_MSK, FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_DLOLB_INT_PER_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_field_RX_LCV_DLOLB_THRSH_set( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_LCV_DLOLB_THRSH_set( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LCV_DLOLB_THRSH_set", A, 12);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_LCV_DLOLB_THRSH_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_LCV_DLOLB_THRSH_set", A, value );

    /* (0x00000024 bits 31:16) bits 0:15 use field RX_LCV_DLOLB_THRSH of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
    fc_cbr_reg_RX_DEFECT_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_MSK,
                                        FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_OFF,
                                        value);
}

static INLINE UINT32 fc_cbr_field_RX_LCV_DLOLB_THRSH_get( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_LCV_DLOLB_THRSH_get( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LCV_DLOLB_THRSH_get", A, 12);
    /* (0x00000024 bits 31:16) bits 0:15 use field RX_LCV_DLOLB_THRSH of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
    reg_value = fc_cbr_reg_RX_DEFECT_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_MSK) >> FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_LCV_DLOLB_THRSH_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_range_RX_LCV_DLOLB_THRSH_set( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_range_RX_LCV_DLOLB_THRSH_set( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_LCV_DLOLB_THRSH_set", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_LCV_DLOLB_THRSH_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_LCV_DLOLB_THRSH_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_LCV_DLOLB_THRSH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000024 bits 31:16) bits 0:15 use field RX_LCV_DLOLB_THRSH of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
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
        /* (0x00000024 bits 31:16) bits 0:15 use field RX_LCV_DLOLB_THRSH of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
        fc_cbr_reg_RX_DEFECT_CFG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_OFF + subfield_offset),
                                            FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_field_range_RX_LCV_DLOLB_THRSH_get( fc_cbr_buffer_t *b_ptr,
                                                                fc800_pmg_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_RX_LCV_DLOLB_THRSH_get( fc_cbr_buffer_t *b_ptr,
                                                                fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_LCV_DLOLB_THRSH_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_LCV_DLOLB_THRSH_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_LCV_DLOLB_THRSH_get", stop_bit, 15 );
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
    /* (0x00000024 bits 31:16) bits 0:15 use field RX_LCV_DLOLB_THRSH of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
    reg_value = fc_cbr_reg_RX_DEFECT_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_MSK)
                  >> FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_MSK, FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_LCV_DLOLB_THRSH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_field_RX_XDET_INTERVAL_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_XDET_INTERVAL_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_XDET_INTERVAL_set", A, 12);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_XDET_INTERVAL_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_XDET_INTERVAL_set", A, value );

    /* (0x00000024 bits 15:8) bits 0:7 use field RX_XDET_INTERVAL of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
    fc_cbr_reg_RX_DEFECT_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_MSK,
                                        FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_OFF,
                                        value);
}

static INLINE UINT32 fc_cbr_field_RX_XDET_INTERVAL_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_XDET_INTERVAL_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_XDET_INTERVAL_get", A, 12);
    /* (0x00000024 bits 15:8) bits 0:7 use field RX_XDET_INTERVAL of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
    reg_value = fc_cbr_reg_RX_DEFECT_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_MSK) >> FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_XDET_INTERVAL_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_range_RX_XDET_INTERVAL_set( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_range_RX_XDET_INTERVAL_set( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_XDET_INTERVAL_set", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_XDET_INTERVAL_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_XDET_INTERVAL_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_XDET_INTERVAL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000024 bits 15:8) bits 0:7 use field RX_XDET_INTERVAL of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
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
        /* (0x00000024 bits 15:8) bits 0:7 use field RX_XDET_INTERVAL of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
        fc_cbr_reg_RX_DEFECT_CFG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_OFF + subfield_offset),
                                            FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_field_range_RX_XDET_INTERVAL_get( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_RX_XDET_INTERVAL_get( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_XDET_INTERVAL_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_XDET_INTERVAL_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_XDET_INTERVAL_get", stop_bit, 7 );
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
    /* (0x00000024 bits 15:8) bits 0:7 use field RX_XDET_INTERVAL of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
    reg_value = fc_cbr_reg_RX_DEFECT_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_MSK)
                  >> FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_MSK, FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_XDET_INTERVAL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_field_RX_XDET_DEASSERT_THRSH_set( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_XDET_DEASSERT_THRSH_set( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_XDET_DEASSERT_THRSH_set", A, 12);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_XDET_DEASSERT_THRSH_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_XDET_DEASSERT_THRSH_set", A, value );

    /* (0x00000024 bits 7:0) bits 0:7 use field RX_XDET_DEASSERT_THRSH of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
    fc_cbr_reg_RX_DEFECT_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_MSK,
                                        FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_OFF,
                                        value);
}

static INLINE UINT32 fc_cbr_field_RX_XDET_DEASSERT_THRSH_get( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_XDET_DEASSERT_THRSH_get( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_XDET_DEASSERT_THRSH_get", A, 12);
    /* (0x00000024 bits 7:0) bits 0:7 use field RX_XDET_DEASSERT_THRSH of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
    reg_value = fc_cbr_reg_RX_DEFECT_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_MSK) >> FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_XDET_DEASSERT_THRSH_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_set( fc_cbr_buffer_t *b_ptr,
                                                                  fc800_pmg_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_set( fc_cbr_buffer_t *b_ptr,
                                                                  fc800_pmg_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_set", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000024 bits 7:0) bits 0:7 use field RX_XDET_DEASSERT_THRSH of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
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
        /* (0x00000024 bits 7:0) bits 0:7 use field RX_XDET_DEASSERT_THRSH of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
        fc_cbr_reg_RX_DEFECT_CFG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_OFF + subfield_offset),
                                            FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_get( fc_cbr_buffer_t *b_ptr,
                                                                    fc800_pmg_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_get( fc_cbr_buffer_t *b_ptr,
                                                                    fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_get", stop_bit, 7 );
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
    /* (0x00000024 bits 7:0) bits 0:7 use field RX_XDET_DEASSERT_THRSH of register PMC_FC_CBR_REG_RX_DEFECT_CFG */
    reg_value = fc_cbr_reg_RX_DEFECT_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_MSK)
                  >> FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_MSK, FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_lfield_TX_USR_PATT_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc_cbr_lfield_TX_USR_PATT_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value[2] )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_lfield_TX_USR_PATT_set", A, 12);
    IOLOG( "%s <= A=%d 0x%08x 0x%08x", "fc_cbr_lfield_TX_USR_PATT_set", A, value[1] , value[0] );

    /* (0x00000040 bits 31:0) bits 0:31 use field TX_USR_PATT of register PMC_FC_CBR_REG_TX_USER_PATTERN_LSB */
    fc_cbr_reg_TX_USER_PATTERN_LSB_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_MSK,
                                              FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_OFF,
                                              value[0]);

    /* (0x00000044 bits 31:24) bits 32:39 use field TX_USR_PATT of register PMC_FC_CBR_REG_TX_CFG */
    fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_MSK,
                                 FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_OFF,
                                 value[1]);
}

static INLINE void fc_cbr_lfield_TX_USR_PATT_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc_cbr_lfield_TX_USR_PATT_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_lfield_TX_USR_PATT_get", A, 12);
    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00000040 bits 31:0) bits 0:31 use field TX_USR_PATT of register PMC_FC_CBR_REG_TX_USER_PATTERN_LSB */
    reg_value = fc_cbr_reg_TX_USER_PATTERN_LSB_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_MSK) >> FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_OFF;
    value[0] |= field_value;

    /* (0x00000044 bits 31:24) bits 32:39 use field TX_USR_PATT of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_MSK) >> FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> A=%d 0x%08x 0x%08x", "fc_cbr_lfield_TX_USR_PATT_get", A, value[1] , value[0] );

}
static INLINE void fc_cbr_lfield_range_TX_USR_PATT_set( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_lfield_range_TX_USR_PATT_set( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_lfield_range_TX_USR_PATT_set", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_lfield_range_TX_USR_PATT_set", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_lfield_range_TX_USR_PATT_set", stop_bit, 39 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_lfield_range_TX_USR_PATT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000040 bits 31:0) bits 0:31 use field TX_USR_PATT of register PMC_FC_CBR_REG_TX_USER_PATTERN_LSB */
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
        /* (0x00000040 bits 31:0) bits 0:31 use field TX_USR_PATT of register PMC_FC_CBR_REG_TX_USER_PATTERN_LSB */
        fc_cbr_reg_TX_USER_PATTERN_LSB_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_OFF + subfield_offset),
                                                  FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000044 bits 31:24) bits 32:39 use field TX_USR_PATT of register PMC_FC_CBR_REG_TX_CFG */
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
        /* (0x00000044 bits 31:24) bits 32:39 use field TX_USR_PATT of register PMC_FC_CBR_REG_TX_CFG */
        fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     subfield_mask << (FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_OFF + subfield_offset),
                                     FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_OFF + subfield_offset,
                                     value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_lfield_range_TX_USR_PATT_get( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_lfield_range_TX_USR_PATT_get( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_lfield_range_TX_USR_PATT_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_lfield_range_TX_USR_PATT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_lfield_range_TX_USR_PATT_get", stop_bit, 39 );
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
    /* (0x00000040 bits 31:0) bits 0:31 use field TX_USR_PATT of register PMC_FC_CBR_REG_TX_USER_PATTERN_LSB */
        reg_value = fc_cbr_reg_TX_USER_PATTERN_LSB_read( b_ptr,
                                                         h_ptr,
                                                         A);
        field_value = (reg_value & FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_MSK)
                      >> FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_MSK, FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_OFF, field_value );
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
    /* (0x00000044 bits 31:24) bits 32:39 use field TX_USR_PATT of register PMC_FC_CBR_REG_TX_CFG */
        reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
        field_value = (reg_value & FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_MSK)
                      >> FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_MSK, FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_lfield_range_TX_USR_PATT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_field_TX_USR_PATT_DISP_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_TX_USR_PATT_DISP_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_USR_PATT_DISP_set", A, 12);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_TX_USR_PATT_DISP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_TX_USR_PATT_DISP_set", A, value );

    /* (0x00000044 bits 19:16) field TX_USR_PATT_DISP of register PMC_FC_CBR_REG_TX_CFG */
    fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_MSK,
                                 FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_TX_USR_PATT_DISP_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_TX_USR_PATT_DISP_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_USR_PATT_DISP_get", A, 12);
    /* (0x00000044 bits 19:16) field TX_USR_PATT_DISP of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_MSK) >> FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_TX_USR_PATT_DISP_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_range_TX_USR_PATT_DISP_set( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_range_TX_USR_PATT_DISP_set( fc_cbr_buffer_t *b_ptr,
                                                            fc800_pmg_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_TX_USR_PATT_DISP_set", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_TX_USR_PATT_DISP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_TX_USR_PATT_DISP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_TX_USR_PATT_DISP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000044 bits 19:16) field TX_USR_PATT_DISP of register PMC_FC_CBR_REG_TX_CFG */
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
        /* (0x00000044 bits 19:16) field TX_USR_PATT_DISP of register PMC_FC_CBR_REG_TX_CFG */
        fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     subfield_mask << (FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_OFF + subfield_offset),
                                     FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_OFF + subfield_offset,
                                     value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_field_range_TX_USR_PATT_DISP_get( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_TX_USR_PATT_DISP_get( fc_cbr_buffer_t *b_ptr,
                                                              fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_TX_USR_PATT_DISP_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_TX_USR_PATT_DISP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_TX_USR_PATT_DISP_get", stop_bit, 3 );
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
    /* (0x00000044 bits 19:16) field TX_USR_PATT_DISP of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_MSK)
                  >> FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_MSK, FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_TX_USR_PATT_DISP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_field_GE_RD_INV_EN_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_GE_RD_INV_EN_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_GE_RD_INV_EN_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_GE_RD_INV_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_GE_RD_INV_EN_set", A, value );

    /* (0x00000044 bits 15) field GE_RD_INV_EN of register PMC_FC_CBR_REG_TX_CFG */
    fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_TX_CFG_BIT_GE_RD_INV_EN_MSK,
                                 FC_CBR_REG_TX_CFG_BIT_GE_RD_INV_EN_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_GE_RD_INV_EN_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_GE_RD_INV_EN_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_GE_RD_INV_EN_get", A, 12);
    /* (0x00000044 bits 15) field GE_RD_INV_EN of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_TX_CFG_BIT_GE_RD_INV_EN_MSK) >> FC_CBR_REG_TX_CFG_BIT_GE_RD_INV_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_GE_RD_INV_EN_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_FC_RD_INV_EN_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_FC_RD_INV_EN_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_FC_RD_INV_EN_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_FC_RD_INV_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_FC_RD_INV_EN_set", A, value );

    /* (0x00000044 bits 14) field FC_RD_INV_EN of register PMC_FC_CBR_REG_TX_CFG */
    fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_TX_CFG_BIT_FC_RD_INV_EN_MSK,
                                 FC_CBR_REG_TX_CFG_BIT_FC_RD_INV_EN_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_FC_RD_INV_EN_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_FC_RD_INV_EN_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_FC_RD_INV_EN_get", A, 12);
    /* (0x00000044 bits 14) field FC_RD_INV_EN of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_TX_CFG_BIT_FC_RD_INV_EN_MSK) >> FC_CBR_REG_TX_CFG_BIT_FC_RD_INV_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_FC_RD_INV_EN_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_TX_RD_INV_EN_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_TX_RD_INV_EN_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_RD_INV_EN_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_TX_RD_INV_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_TX_RD_INV_EN_set", A, value );

    /* (0x00000044 bits 13) field TX_RD_INV_EN of register PMC_FC_CBR_REG_TX_CFG */
    fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_TX_CFG_BIT_TX_RD_INV_EN_MSK,
                                 FC_CBR_REG_TX_CFG_BIT_TX_RD_INV_EN_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_TX_RD_INV_EN_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_TX_RD_INV_EN_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_RD_INV_EN_get", A, 12);
    /* (0x00000044 bits 13) field TX_RD_INV_EN of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_TX_CFG_BIT_TX_RD_INV_EN_MSK) >> FC_CBR_REG_TX_CFG_BIT_TX_RD_INV_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_TX_RD_INV_EN_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_TX_LCV_ERR_INS_EN_set( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_TX_LCV_ERR_INS_EN_set( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_LCV_ERR_INS_EN_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_TX_LCV_ERR_INS_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_TX_LCV_ERR_INS_EN_set", A, value );

    /* (0x00000044 bits 7) field TX_LCV_ERR_INS_EN of register PMC_FC_CBR_REG_TX_CFG */
    fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_TX_CFG_BIT_TX_LCV_ERR_INS_EN_MSK,
                                 FC_CBR_REG_TX_CFG_BIT_TX_LCV_ERR_INS_EN_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_TX_LCV_ERR_INS_EN_get( fc_cbr_buffer_t *b_ptr,
                                                         fc800_pmg_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_TX_LCV_ERR_INS_EN_get( fc_cbr_buffer_t *b_ptr,
                                                         fc800_pmg_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_LCV_ERR_INS_EN_get", A, 12);
    /* (0x00000044 bits 7) field TX_LCV_ERR_INS_EN of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_TX_CFG_BIT_TX_LCV_ERR_INS_EN_MSK) >> FC_CBR_REG_TX_CFG_BIT_TX_LCV_ERR_INS_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_TX_LCV_ERR_INS_EN_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_TX_PN11_INV_set( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_TX_PN11_INV_set( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_PN11_INV_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_TX_PN11_INV_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_TX_PN11_INV_set", A, value );

    /* (0x00000044 bits 6) field TX_PN11_INV of register PMC_FC_CBR_REG_TX_CFG */
    fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_TX_CFG_BIT_TX_PN11_INV_MSK,
                                 FC_CBR_REG_TX_CFG_BIT_TX_PN11_INV_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_TX_PN11_INV_get( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_TX_PN11_INV_get( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_PN11_INV_get", A, 12);
    /* (0x00000044 bits 6) field TX_PN11_INV of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_TX_CFG_BIT_TX_PN11_INV_MSK) >> FC_CBR_REG_TX_CFG_BIT_TX_PN11_INV_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_TX_PN11_INV_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_TX_NOS_PN11_SEL_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_TX_NOS_PN11_SEL_set( fc_cbr_buffer_t *b_ptr,
                                                     fc800_pmg_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_NOS_PN11_SEL_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_TX_NOS_PN11_SEL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_TX_NOS_PN11_SEL_set", A, value );

    /* (0x00000044 bits 5) field TX_NOS_PN11_SEL of register PMC_FC_CBR_REG_TX_CFG */
    fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_TX_CFG_BIT_TX_NOS_PN11_SEL_MSK,
                                 FC_CBR_REG_TX_CFG_BIT_TX_NOS_PN11_SEL_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_TX_NOS_PN11_SEL_get( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_TX_NOS_PN11_SEL_get( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_NOS_PN11_SEL_get", A, 12);
    /* (0x00000044 bits 5) field TX_NOS_PN11_SEL of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_TX_CFG_BIT_TX_NOS_PN11_SEL_MSK) >> FC_CBR_REG_TX_CFG_BIT_TX_NOS_PN11_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_TX_NOS_PN11_SEL_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_TX_FORCE_NOS_PN11_set( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_TX_FORCE_NOS_PN11_set( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_FORCE_NOS_PN11_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_TX_FORCE_NOS_PN11_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_TX_FORCE_NOS_PN11_set", A, value );

    /* (0x00000044 bits 4) field TX_FORCE_NOS_PN11 of register PMC_FC_CBR_REG_TX_CFG */
    fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_TX_CFG_BIT_TX_FORCE_NOS_PN11_MSK,
                                 FC_CBR_REG_TX_CFG_BIT_TX_FORCE_NOS_PN11_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_TX_FORCE_NOS_PN11_get( fc_cbr_buffer_t *b_ptr,
                                                         fc800_pmg_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_TX_FORCE_NOS_PN11_get( fc_cbr_buffer_t *b_ptr,
                                                         fc800_pmg_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_FORCE_NOS_PN11_get", A, 12);
    /* (0x00000044 bits 4) field TX_FORCE_NOS_PN11 of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_TX_CFG_BIT_TX_FORCE_NOS_PN11_MSK) >> FC_CBR_REG_TX_CFG_BIT_TX_FORCE_NOS_PN11_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_TX_FORCE_NOS_PN11_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_TX_SOFT_RST_set( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_TX_SOFT_RST_set( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_SOFT_RST_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_TX_SOFT_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_TX_SOFT_RST_set", A, value );

    /* (0x00000044 bits 0) field TX_SOFT_RST of register PMC_FC_CBR_REG_TX_CFG */
    fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_TX_CFG_BIT_TX_SOFT_RST_MSK,
                                 FC_CBR_REG_TX_CFG_BIT_TX_SOFT_RST_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_TX_SOFT_RST_get( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_TX_SOFT_RST_get( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_SOFT_RST_get", A, 12);
    /* (0x00000044 bits 0) field TX_SOFT_RST of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_TX_CFG_BIT_TX_SOFT_RST_MSK) >> FC_CBR_REG_TX_CFG_BIT_TX_SOFT_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_TX_SOFT_RST_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void fc_cbr_field_RX_FORCE_REALIGN_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_FORCE_REALIGN_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_FORCE_REALIGN_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_FORCE_REALIGN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_FORCE_REALIGN_set", A, value );

    /* (0x00000014 bits 1) field RX_FORCE_REALIGN of register PMC_FC_CBR_REG_RX_CFG */
    fc_cbr_reg_RX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_RX_CFG_BIT_RX_FORCE_REALIGN_MSK,
                                 FC_CBR_REG_RX_CFG_BIT_RX_FORCE_REALIGN_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_RX_FORCE_REALIGN_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_FORCE_REALIGN_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_FORCE_REALIGN_get", A, 12);
    /* (0x00000014 bits 1) field RX_FORCE_REALIGN of register PMC_FC_CBR_REG_RX_CFG */
    reg_value = fc_cbr_reg_RX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_CFG_BIT_RX_FORCE_REALIGN_MSK) >> FC_CBR_REG_RX_CFG_BIT_RX_FORCE_REALIGN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_FORCE_REALIGN_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_TX_USR_PATT_UPDATE_set( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_TX_USR_PATT_UPDATE_set( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_USR_PATT_UPDATE_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_TX_USR_PATT_UPDATE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_TX_USR_PATT_UPDATE_set", A, value );

    /* (0x00000044 bits 9) field TX_USR_PATT_UPDATE of register PMC_FC_CBR_REG_TX_CFG */
    fc_cbr_reg_TX_CFG_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_UPDATE_MSK,
                                 FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_UPDATE_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_TX_USR_PATT_UPDATE_get( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_TX_USR_PATT_UPDATE_get( fc_cbr_buffer_t *b_ptr,
                                                          fc800_pmg_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_TX_USR_PATT_UPDATE_get", A, 12);
    /* (0x00000044 bits 9) field TX_USR_PATT_UPDATE of register PMC_FC_CBR_REG_TX_CFG */
    reg_value = fc_cbr_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_UPDATE_MSK) >> FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_UPDATE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_TX_USR_PATT_UPDATE_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_CNTR_UPDATE_set( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_CNTR_UPDATE_set( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_CNTR_UPDATE_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_CNTR_UPDATE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_CNTR_UPDATE_set", A, value );

    /* (0x00000094 bits 0) field CNTR_UPDATE of register PMC_FC_CBR_REG_CNT_UPDATE */
    fc_cbr_reg_CNT_UPDATE_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     FC_CBR_REG_CNT_UPDATE_BIT_CNTR_UPDATE_MSK,
                                     FC_CBR_REG_CNT_UPDATE_BIT_CNTR_UPDATE_OFF,
                                     value);
}

static INLINE UINT32 fc_cbr_field_CNTR_UPDATE_get( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_CNTR_UPDATE_get( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_CNTR_UPDATE_get", A, 12);
    /* (0x00000094 bits 0) field CNTR_UPDATE of register PMC_FC_CBR_REG_CNT_UPDATE */
    reg_value = fc_cbr_reg_CNT_UPDATE_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_CNT_UPDATE_BIT_CNTR_UPDATE_MSK) >> FC_CBR_REG_CNT_UPDATE_BIT_CNTR_UPDATE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_CNTR_UPDATE_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_both
 * ==================================================================================
 */
static INLINE void fc_cbr_field_RX_DISP_INVERT_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                               fc800_pmg_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_DISP_INVERT_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                               fc800_pmg_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DISP_INVERT_I_set_to_clear", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_DISP_INVERT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_DISP_INVERT_I_set_to_clear", A, value );

    /* (0x000000b4 bits 6) field RX_DISP_INVERT_I of register PMC_FC_CBR_REG_INT */
    fc_cbr_reg_INT_action_on_write_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_CBR_REG_INT_BIT_RX_DISP_INVERT_I_MSK,
                                              FC_CBR_REG_INT_BIT_RX_DISP_INVERT_I_OFF,
                                              value);
}

static INLINE UINT32 fc_cbr_field_RX_DISP_INVERT_I_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_DISP_INVERT_I_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DISP_INVERT_I_get", A, 12);
    /* (0x000000b4 bits 6) field RX_DISP_INVERT_I of register PMC_FC_CBR_REG_INT */
    reg_value = fc_cbr_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_BIT_RX_DISP_INVERT_I_MSK) >> FC_CBR_REG_INT_BIT_RX_DISP_INVERT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_DISP_INVERT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DISP_INVERT_I_poll( fc_cbr_buffer_t *b_ptr,
                                                                       fc800_pmg_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DISP_INVERT_I_poll( fc_cbr_buffer_t *b_ptr,
                                                                       fc800_pmg_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_DISP_INVERT_I_poll", A, value );

    /* (0x000000b4 bits 6) field RX_DISP_INVERT_I of register PMC_FC_CBR_REG_INT */
    return fc_cbr_reg_INT_poll( b_ptr,
                                h_ptr,
                                A,
                                FC_CBR_REG_INT_BIT_RX_DISP_INVERT_I_MSK,
                                (value<<FC_CBR_REG_INT_BIT_RX_DISP_INVERT_I_OFF),
                                cmp,
                                max_count,
                                num_failed_polls,
                                delay_between_polls_in_microseconds);
}

static INLINE void fc_cbr_field_RX_XDET_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_XDET_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_XDET_I_set_to_clear", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_XDET_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_XDET_I_set_to_clear", A, value );

    /* (0x000000b4 bits 5) field RX_XDET_I of register PMC_FC_CBR_REG_INT */
    fc_cbr_reg_INT_action_on_write_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_CBR_REG_INT_BIT_RX_XDET_I_MSK,
                                              FC_CBR_REG_INT_BIT_RX_XDET_I_OFF,
                                              value);
}

static INLINE UINT32 fc_cbr_field_RX_XDET_I_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_XDET_I_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_XDET_I_get", A, 12);
    /* (0x000000b4 bits 5) field RX_XDET_I of register PMC_FC_CBR_REG_INT */
    reg_value = fc_cbr_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_BIT_RX_XDET_I_MSK) >> FC_CBR_REG_INT_BIT_RX_XDET_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_XDET_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_XDET_I_poll( fc_cbr_buffer_t *b_ptr,
                                                                fc800_pmg_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_XDET_I_poll( fc_cbr_buffer_t *b_ptr,
                                                                fc800_pmg_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_XDET_I_poll", A, value );

    /* (0x000000b4 bits 5) field RX_XDET_I of register PMC_FC_CBR_REG_INT */
    return fc_cbr_reg_INT_poll( b_ptr,
                                h_ptr,
                                A,
                                FC_CBR_REG_INT_BIT_RX_XDET_I_MSK,
                                (value<<FC_CBR_REG_INT_BIT_RX_XDET_I_OFF),
                                cmp,
                                max_count,
                                num_failed_polls,
                                delay_between_polls_in_microseconds);
}

static INLINE void fc_cbr_field_RX_DLOLB_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                         fc800_pmg_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_DLOLB_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                         fc800_pmg_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DLOLB_I_set_to_clear", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_DLOLB_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_DLOLB_I_set_to_clear", A, value );

    /* (0x000000b4 bits 3) field RX_DLOLB_I of register PMC_FC_CBR_REG_INT */
    fc_cbr_reg_INT_action_on_write_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_CBR_REG_INT_BIT_RX_DLOLB_I_MSK,
                                              FC_CBR_REG_INT_BIT_RX_DLOLB_I_OFF,
                                              value);
}

static INLINE UINT32 fc_cbr_field_RX_DLOLB_I_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_DLOLB_I_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DLOLB_I_get", A, 12);
    /* (0x000000b4 bits 3) field RX_DLOLB_I of register PMC_FC_CBR_REG_INT */
    reg_value = fc_cbr_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_BIT_RX_DLOLB_I_MSK) >> FC_CBR_REG_INT_BIT_RX_DLOLB_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_DLOLB_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DLOLB_I_poll( fc_cbr_buffer_t *b_ptr,
                                                                 fc800_pmg_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DLOLB_I_poll( fc_cbr_buffer_t *b_ptr,
                                                                 fc800_pmg_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_DLOLB_I_poll", A, value );

    /* (0x000000b4 bits 3) field RX_DLOLB_I of register PMC_FC_CBR_REG_INT */
    return fc_cbr_reg_INT_poll( b_ptr,
                                h_ptr,
                                A,
                                FC_CBR_REG_INT_BIT_RX_DLOLB_I_MSK,
                                (value<<FC_CBR_REG_INT_BIT_RX_DLOLB_I_OFF),
                                cmp,
                                max_count,
                                num_failed_polls,
                                delay_between_polls_in_microseconds);
}

static INLINE void fc_cbr_field_RX_ASD_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_ASD_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_ASD_I_set_to_clear", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_ASD_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_ASD_I_set_to_clear", A, value );

    /* (0x000000b4 bits 2) field RX_ASD_I of register PMC_FC_CBR_REG_INT */
    fc_cbr_reg_INT_action_on_write_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_CBR_REG_INT_BIT_RX_ASD_I_MSK,
                                              FC_CBR_REG_INT_BIT_RX_ASD_I_OFF,
                                              value);
}

static INLINE UINT32 fc_cbr_field_RX_ASD_I_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_ASD_I_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_ASD_I_get", A, 12);
    /* (0x000000b4 bits 2) field RX_ASD_I of register PMC_FC_CBR_REG_INT */
    reg_value = fc_cbr_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_BIT_RX_ASD_I_MSK) >> FC_CBR_REG_INT_BIT_RX_ASD_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_ASD_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_ASD_I_poll( fc_cbr_buffer_t *b_ptr,
                                                               fc800_pmg_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_ASD_I_poll( fc_cbr_buffer_t *b_ptr,
                                                               fc800_pmg_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_ASD_I_poll", A, value );

    /* (0x000000b4 bits 2) field RX_ASD_I of register PMC_FC_CBR_REG_INT */
    return fc_cbr_reg_INT_poll( b_ptr,
                                h_ptr,
                                A,
                                FC_CBR_REG_INT_BIT_RX_ASD_I_MSK,
                                (value<<FC_CBR_REG_INT_BIT_RX_ASD_I_OFF),
                                cmp,
                                max_count,
                                num_failed_polls,
                                delay_between_polls_in_microseconds);
}

static INLINE void fc_cbr_field_RX_SYNC_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_SYNC_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_SYNC_I_set_to_clear", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_SYNC_I_set_to_clear", A, value );

    /* (0x000000b4 bits 1) field RX_SYNC_I of register PMC_FC_CBR_REG_INT */
    fc_cbr_reg_INT_action_on_write_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_CBR_REG_INT_BIT_RX_SYNC_I_MSK,
                                              FC_CBR_REG_INT_BIT_RX_SYNC_I_OFF,
                                              value);
}

static INLINE UINT32 fc_cbr_field_RX_SYNC_I_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_SYNC_I_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_SYNC_I_get", A, 12);
    /* (0x000000b4 bits 1) field RX_SYNC_I of register PMC_FC_CBR_REG_INT */
    reg_value = fc_cbr_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_BIT_RX_SYNC_I_MSK) >> FC_CBR_REG_INT_BIT_RX_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_SYNC_I_poll( fc_cbr_buffer_t *b_ptr,
                                                                fc800_pmg_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_SYNC_I_poll( fc_cbr_buffer_t *b_ptr,
                                                                fc800_pmg_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_SYNC_I_poll", A, value );

    /* (0x000000b4 bits 1) field RX_SYNC_I of register PMC_FC_CBR_REG_INT */
    return fc_cbr_reg_INT_poll( b_ptr,
                                h_ptr,
                                A,
                                FC_CBR_REG_INT_BIT_RX_SYNC_I_MSK,
                                (value<<FC_CBR_REG_INT_BIT_RX_SYNC_I_OFF),
                                cmp,
                                max_count,
                                num_failed_polls,
                                delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */
static INLINE void fc_cbr_field_RX_SOF_DET_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                           fc800_pmg_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_SOF_DET_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                           fc800_pmg_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_SOF_DET_I_set_to_clear", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_SOF_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_SOF_DET_I_set_to_clear", A, value );

    /* (0x000000b4 bits 4) field RX_SOF_DET_I of register PMC_FC_CBR_REG_INT */
    fc_cbr_reg_INT_action_on_write_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_CBR_REG_INT_BIT_RX_SOF_DET_I_MSK,
                                              FC_CBR_REG_INT_BIT_RX_SOF_DET_I_OFF,
                                              value);
}

static INLINE UINT32 fc_cbr_field_RX_SOF_DET_I_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_SOF_DET_I_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_SOF_DET_I_get", A, 12);
    /* (0x000000b4 bits 4) field RX_SOF_DET_I of register PMC_FC_CBR_REG_INT */
    reg_value = fc_cbr_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_BIT_RX_SOF_DET_I_MSK) >> FC_CBR_REG_INT_BIT_RX_SOF_DET_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_SOF_DET_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_SOF_DET_I_poll( fc_cbr_buffer_t *b_ptr,
                                                                   fc800_pmg_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_SOF_DET_I_poll( fc_cbr_buffer_t *b_ptr,
                                                                   fc800_pmg_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_SOF_DET_I_poll", A, value );

    /* (0x000000b4 bits 4) field RX_SOF_DET_I of register PMC_FC_CBR_REG_INT */
    return fc_cbr_reg_INT_poll( b_ptr,
                                h_ptr,
                                A,
                                FC_CBR_REG_INT_BIT_RX_SOF_DET_I_MSK,
                                (value<<FC_CBR_REG_INT_BIT_RX_SOF_DET_I_OFF),
                                cmp,
                                max_count,
                                num_failed_polls,
                                delay_between_polls_in_microseconds);
}

static INLINE void fc_cbr_field_RX_LCV_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_LCV_I_set_to_clear( fc_cbr_buffer_t *b_ptr,
                                                       fc800_pmg_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LCV_I_set_to_clear", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_LCV_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_LCV_I_set_to_clear", A, value );

    /* (0x000000b4 bits 0) field RX_LCV_I of register PMC_FC_CBR_REG_INT */
    fc_cbr_reg_INT_action_on_write_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_CBR_REG_INT_BIT_RX_LCV_I_MSK,
                                              FC_CBR_REG_INT_BIT_RX_LCV_I_OFF,
                                              value);
}

static INLINE UINT32 fc_cbr_field_RX_LCV_I_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_LCV_I_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LCV_I_get", A, 12);
    /* (0x000000b4 bits 0) field RX_LCV_I of register PMC_FC_CBR_REG_INT */
    reg_value = fc_cbr_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_BIT_RX_LCV_I_MSK) >> FC_CBR_REG_INT_BIT_RX_LCV_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_LCV_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_LCV_I_poll( fc_cbr_buffer_t *b_ptr,
                                                               fc800_pmg_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_LCV_I_poll( fc_cbr_buffer_t *b_ptr,
                                                               fc800_pmg_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_LCV_I_poll", A, value );

    /* (0x000000b4 bits 0) field RX_LCV_I of register PMC_FC_CBR_REG_INT */
    return fc_cbr_reg_INT_poll( b_ptr,
                                h_ptr,
                                A,
                                FC_CBR_REG_INT_BIT_RX_LCV_I_MSK,
                                (value<<FC_CBR_REG_INT_BIT_RX_LCV_I_OFF),
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
static INLINE void fc_cbr_field_RX_DISP_INVERT_E_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_DISP_INVERT_E_set( fc_cbr_buffer_t *b_ptr,
                                                      fc800_pmg_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DISP_INVERT_E_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_DISP_INVERT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_DISP_INVERT_E_set", A, value );

    /* (0x000000b0 bits 6) field RX_DISP_INVERT_E of register PMC_FC_CBR_REG_INT_EN */
    fc_cbr_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_INT_EN_BIT_RX_DISP_INVERT_E_MSK,
                                 FC_CBR_REG_INT_EN_BIT_RX_DISP_INVERT_E_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_RX_DISP_INVERT_E_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_DISP_INVERT_E_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DISP_INVERT_E_get", A, 12);
    /* (0x000000b0 bits 6) field RX_DISP_INVERT_E of register PMC_FC_CBR_REG_INT_EN */
    reg_value = fc_cbr_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_EN_BIT_RX_DISP_INVERT_E_MSK) >> FC_CBR_REG_INT_EN_BIT_RX_DISP_INVERT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_DISP_INVERT_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_XDET_E_set( fc_cbr_buffer_t *b_ptr,
                                               fc800_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_XDET_E_set( fc_cbr_buffer_t *b_ptr,
                                               fc800_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_XDET_E_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_XDET_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_XDET_E_set", A, value );

    /* (0x000000b0 bits 5) field RX_XDET_E of register PMC_FC_CBR_REG_INT_EN */
    fc_cbr_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_INT_EN_BIT_RX_XDET_E_MSK,
                                 FC_CBR_REG_INT_EN_BIT_RX_XDET_E_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_RX_XDET_E_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_XDET_E_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_XDET_E_get", A, 12);
    /* (0x000000b0 bits 5) field RX_XDET_E of register PMC_FC_CBR_REG_INT_EN */
    reg_value = fc_cbr_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_EN_BIT_RX_XDET_E_MSK) >> FC_CBR_REG_INT_EN_BIT_RX_XDET_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_XDET_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_SOF_DET_E_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_SOF_DET_E_set( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_SOF_DET_E_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_SOF_DET_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_SOF_DET_E_set", A, value );

    /* (0x000000b0 bits 4) field RX_SOF_DET_E of register PMC_FC_CBR_REG_INT_EN */
    fc_cbr_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_INT_EN_BIT_RX_SOF_DET_E_MSK,
                                 FC_CBR_REG_INT_EN_BIT_RX_SOF_DET_E_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_RX_SOF_DET_E_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_SOF_DET_E_get( fc_cbr_buffer_t *b_ptr,
                                                    fc800_pmg_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_SOF_DET_E_get", A, 12);
    /* (0x000000b0 bits 4) field RX_SOF_DET_E of register PMC_FC_CBR_REG_INT_EN */
    reg_value = fc_cbr_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_EN_BIT_RX_SOF_DET_E_MSK) >> FC_CBR_REG_INT_EN_BIT_RX_SOF_DET_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_SOF_DET_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_DLOLB_E_set( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_DLOLB_E_set( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DLOLB_E_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_DLOLB_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_DLOLB_E_set", A, value );

    /* (0x000000b0 bits 3) field RX_DLOLB_E of register PMC_FC_CBR_REG_INT_EN */
    fc_cbr_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_INT_EN_BIT_RX_DLOLB_E_MSK,
                                 FC_CBR_REG_INT_EN_BIT_RX_DLOLB_E_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_RX_DLOLB_E_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_DLOLB_E_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DLOLB_E_get", A, 12);
    /* (0x000000b0 bits 3) field RX_DLOLB_E of register PMC_FC_CBR_REG_INT_EN */
    reg_value = fc_cbr_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_EN_BIT_RX_DLOLB_E_MSK) >> FC_CBR_REG_INT_EN_BIT_RX_DLOLB_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_DLOLB_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_ASD_E_set( fc_cbr_buffer_t *b_ptr,
                                              fc800_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_ASD_E_set( fc_cbr_buffer_t *b_ptr,
                                              fc800_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_ASD_E_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_ASD_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_ASD_E_set", A, value );

    /* (0x000000b0 bits 2) field RX_ASD_E of register PMC_FC_CBR_REG_INT_EN */
    fc_cbr_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_INT_EN_BIT_RX_ASD_E_MSK,
                                 FC_CBR_REG_INT_EN_BIT_RX_ASD_E_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_RX_ASD_E_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_ASD_E_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_ASD_E_get", A, 12);
    /* (0x000000b0 bits 2) field RX_ASD_E of register PMC_FC_CBR_REG_INT_EN */
    reg_value = fc_cbr_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_EN_BIT_RX_ASD_E_MSK) >> FC_CBR_REG_INT_EN_BIT_RX_ASD_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_ASD_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_SYNC_E_set( fc_cbr_buffer_t *b_ptr,
                                               fc800_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_SYNC_E_set( fc_cbr_buffer_t *b_ptr,
                                               fc800_pmg_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_SYNC_E_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_SYNC_E_set", A, value );

    /* (0x000000b0 bits 1) field RX_SYNC_E of register PMC_FC_CBR_REG_INT_EN */
    fc_cbr_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_INT_EN_BIT_RX_SYNC_E_MSK,
                                 FC_CBR_REG_INT_EN_BIT_RX_SYNC_E_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_RX_SYNC_E_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_SYNC_E_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_SYNC_E_get", A, 12);
    /* (0x000000b0 bits 1) field RX_SYNC_E of register PMC_FC_CBR_REG_INT_EN */
    reg_value = fc_cbr_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_EN_BIT_RX_SYNC_E_MSK) >> FC_CBR_REG_INT_EN_BIT_RX_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_SYNC_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_field_RX_LCV_E_set( fc_cbr_buffer_t *b_ptr,
                                              fc800_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_field_RX_LCV_E_set( fc_cbr_buffer_t *b_ptr,
                                              fc800_pmg_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LCV_E_set", A, 12);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_field_RX_LCV_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_field_RX_LCV_E_set", A, value );

    /* (0x000000b0 bits 0) field RX_LCV_E of register PMC_FC_CBR_REG_INT_EN */
    fc_cbr_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_CBR_REG_INT_EN_BIT_RX_LCV_E_MSK,
                                 FC_CBR_REG_INT_EN_BIT_RX_LCV_E_OFF,
                                 value);
}

static INLINE UINT32 fc_cbr_field_RX_LCV_E_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_LCV_E_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LCV_E_get", A, 12);
    /* (0x000000b0 bits 0) field RX_LCV_E of register PMC_FC_CBR_REG_INT_EN */
    reg_value = fc_cbr_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_EN_BIT_RX_LCV_E_MSK) >> FC_CBR_REG_INT_EN_BIT_RX_LCV_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_LCV_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 fc_cbr_field_RX_DISP_INVERT_V_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_DISP_INVERT_V_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DISP_INVERT_V_get", A, 12);
    /* (0x000000b8 bits 6) field RX_DISP_INVERT_V of register PMC_FC_CBR_REG_INT_VAL */
    reg_value = fc_cbr_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_VAL_BIT_RX_DISP_INVERT_V_MSK) >> FC_CBR_REG_INT_VAL_BIT_RX_DISP_INVERT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_DISP_INVERT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DISP_INVERT_V_poll( fc_cbr_buffer_t *b_ptr,
                                                                       fc800_pmg_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DISP_INVERT_V_poll( fc_cbr_buffer_t *b_ptr,
                                                                       fc800_pmg_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_DISP_INVERT_V_poll", A, value );

    /* (0x000000b8 bits 6) field RX_DISP_INVERT_V of register PMC_FC_CBR_REG_INT_VAL */
    return fc_cbr_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    FC_CBR_REG_INT_VAL_BIT_RX_DISP_INVERT_V_MSK,
                                    (value<<FC_CBR_REG_INT_VAL_BIT_RX_DISP_INVERT_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc_cbr_field_RX_XDET_V_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_XDET_V_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_XDET_V_get", A, 12);
    /* (0x000000b8 bits 5) field RX_XDET_V of register PMC_FC_CBR_REG_INT_VAL */
    reg_value = fc_cbr_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_VAL_BIT_RX_XDET_V_MSK) >> FC_CBR_REG_INT_VAL_BIT_RX_XDET_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_XDET_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_XDET_V_poll( fc_cbr_buffer_t *b_ptr,
                                                                fc800_pmg_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_XDET_V_poll( fc_cbr_buffer_t *b_ptr,
                                                                fc800_pmg_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_XDET_V_poll", A, value );

    /* (0x000000b8 bits 5) field RX_XDET_V of register PMC_FC_CBR_REG_INT_VAL */
    return fc_cbr_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    FC_CBR_REG_INT_VAL_BIT_RX_XDET_V_MSK,
                                    (value<<FC_CBR_REG_INT_VAL_BIT_RX_XDET_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc_cbr_field_RX_DLOLB_V_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_DLOLB_V_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_DLOLB_V_get", A, 12);
    /* (0x000000b8 bits 3) field RX_DLOLB_V of register PMC_FC_CBR_REG_INT_VAL */
    reg_value = fc_cbr_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_VAL_BIT_RX_DLOLB_V_MSK) >> FC_CBR_REG_INT_VAL_BIT_RX_DLOLB_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_DLOLB_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DLOLB_V_poll( fc_cbr_buffer_t *b_ptr,
                                                                 fc800_pmg_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DLOLB_V_poll( fc_cbr_buffer_t *b_ptr,
                                                                 fc800_pmg_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_DLOLB_V_poll", A, value );

    /* (0x000000b8 bits 3) field RX_DLOLB_V of register PMC_FC_CBR_REG_INT_VAL */
    return fc_cbr_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    FC_CBR_REG_INT_VAL_BIT_RX_DLOLB_V_MSK,
                                    (value<<FC_CBR_REG_INT_VAL_BIT_RX_DLOLB_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc_cbr_field_RX_ASD_V_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_ASD_V_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_ASD_V_get", A, 12);
    /* (0x000000b8 bits 2) field RX_ASD_V of register PMC_FC_CBR_REG_INT_VAL */
    reg_value = fc_cbr_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_VAL_BIT_RX_ASD_V_MSK) >> FC_CBR_REG_INT_VAL_BIT_RX_ASD_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_ASD_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_ASD_V_poll( fc_cbr_buffer_t *b_ptr,
                                                               fc800_pmg_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_ASD_V_poll( fc_cbr_buffer_t *b_ptr,
                                                               fc800_pmg_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_ASD_V_poll", A, value );

    /* (0x000000b8 bits 2) field RX_ASD_V of register PMC_FC_CBR_REG_INT_VAL */
    return fc_cbr_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    FC_CBR_REG_INT_VAL_BIT_RX_ASD_V_MSK,
                                    (value<<FC_CBR_REG_INT_VAL_BIT_RX_ASD_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc_cbr_field_RX_SYNC_V_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_SYNC_V_get( fc_cbr_buffer_t *b_ptr,
                                                 fc800_pmg_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_SYNC_V_get", A, 12);
    /* (0x000000b8 bits 1) field RX_SYNC_V of register PMC_FC_CBR_REG_INT_VAL */
    reg_value = fc_cbr_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_VAL_BIT_RX_SYNC_V_MSK) >> FC_CBR_REG_INT_VAL_BIT_RX_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_SYNC_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_SYNC_V_poll( fc_cbr_buffer_t *b_ptr,
                                                                fc800_pmg_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_SYNC_V_poll( fc_cbr_buffer_t *b_ptr,
                                                                fc800_pmg_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_SYNC_V_poll", A, value );

    /* (0x000000b8 bits 1) field RX_SYNC_V of register PMC_FC_CBR_REG_INT_VAL */
    return fc_cbr_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    FC_CBR_REG_INT_VAL_BIT_RX_SYNC_V_MSK,
                                    (value<<FC_CBR_REG_INT_VAL_BIT_RX_SYNC_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc_cbr_field_RX_LCV_V_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_LCV_V_get( fc_cbr_buffer_t *b_ptr,
                                                fc800_pmg_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LCV_V_get", A, 12);
    /* (0x000000b8 bits 0) field RX_LCV_V of register PMC_FC_CBR_REG_INT_VAL */
    reg_value = fc_cbr_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_INT_VAL_BIT_RX_LCV_V_MSK) >> FC_CBR_REG_INT_VAL_BIT_RX_LCV_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_LCV_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_LCV_V_poll( fc_cbr_buffer_t *b_ptr,
                                                               fc800_pmg_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_field_RX_LCV_V_poll( fc_cbr_buffer_t *b_ptr,
                                                               fc800_pmg_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_field_RX_LCV_V_poll", A, value );

    /* (0x000000b8 bits 0) field RX_LCV_V of register PMC_FC_CBR_REG_INT_VAL */
    return fc_cbr_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    FC_CBR_REG_INT_VAL_BIT_RX_LCV_V_MSK,
                                    (value<<FC_CBR_REG_INT_VAL_BIT_RX_LCV_V_OFF),
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
static INLINE UINT32 fc_cbr_field_RX_GF_DWRD_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_GF_DWRD_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_GF_DWRD_get", A, 12);
    /* (0x00000028 bits 31:0) field RX_GF_DWRD of register PMC_FC_CBR_REG_RX_GF_WRD_CNTR */
    reg_value = fc_cbr_reg_RX_GF_WRD_CNTR_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_GF_WRD_CNTR_BIT_RX_GF_DWRD_MSK) >> FC_CBR_REG_RX_GF_WRD_CNTR_BIT_RX_GF_DWRD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_GF_DWRD_get", A, value );

    return value;
}
static INLINE UINT32 fc_cbr_field_range_RX_GF_DWRD_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_RX_GF_DWRD_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_GF_DWRD_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_GF_DWRD_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_GF_DWRD_get", stop_bit, 31 );
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
    /* (0x00000028 bits 31:0) field RX_GF_DWRD of register PMC_FC_CBR_REG_RX_GF_WRD_CNTR */
    reg_value = fc_cbr_reg_RX_GF_WRD_CNTR_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_RX_GF_WRD_CNTR_BIT_RX_GF_DWRD_MSK)
                  >> FC_CBR_REG_RX_GF_WRD_CNTR_BIT_RX_GF_DWRD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_RX_GF_WRD_CNTR_BIT_RX_GF_DWRD_MSK, FC_CBR_REG_RX_GF_WRD_CNTR_BIT_RX_GF_DWRD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_GF_DWRD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc_cbr_field_RX_BF_DWRD_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_BF_DWRD_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_BF_DWRD_get", A, 12);
    /* (0x0000002c bits 31:0) field RX_BF_DWRD of register PMC_FC_CBR_REG_RX_BF_WRD_CNTR */
    reg_value = fc_cbr_reg_RX_BF_WRD_CNTR_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_BF_WRD_CNTR_BIT_RX_BF_DWRD_MSK) >> FC_CBR_REG_RX_BF_WRD_CNTR_BIT_RX_BF_DWRD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_BF_DWRD_get", A, value );

    return value;
}
static INLINE UINT32 fc_cbr_field_range_RX_BF_DWRD_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_RX_BF_DWRD_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_BF_DWRD_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_BF_DWRD_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_BF_DWRD_get", stop_bit, 31 );
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
    /* (0x0000002c bits 31:0) field RX_BF_DWRD of register PMC_FC_CBR_REG_RX_BF_WRD_CNTR */
    reg_value = fc_cbr_reg_RX_BF_WRD_CNTR_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_RX_BF_WRD_CNTR_BIT_RX_BF_DWRD_MSK)
                  >> FC_CBR_REG_RX_BF_WRD_CNTR_BIT_RX_BF_DWRD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_RX_BF_WRD_CNTR_BIT_RX_BF_DWRD_MSK, FC_CBR_REG_RX_BF_WRD_CNTR_BIT_RX_BF_DWRD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_BF_DWRD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc_cbr_field_RX_WRD_CNT_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_WRD_CNT_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_WRD_CNT_get", A, 12);
    /* (0x00000030 bits 31:0) bits 0:31 use field RX_WRD_CNT of register PMC_FC_CBR_REG_RX_WORD_CNT */
    reg_value = fc_cbr_reg_RX_WORD_CNT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_WORD_CNT_BIT_RX_WRD_CNT_MSK) >> FC_CBR_REG_RX_WORD_CNT_BIT_RX_WRD_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_WRD_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc_cbr_field_range_RX_WRD_CNT_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_RX_WRD_CNT_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_WRD_CNT_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_WRD_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_WRD_CNT_get", stop_bit, 31 );
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
    /* (0x00000030 bits 31:0) bits 0:31 use field RX_WRD_CNT of register PMC_FC_CBR_REG_RX_WORD_CNT */
    reg_value = fc_cbr_reg_RX_WORD_CNT_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_RX_WORD_CNT_BIT_RX_WRD_CNT_MSK)
                  >> FC_CBR_REG_RX_WORD_CNT_BIT_RX_WRD_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_RX_WORD_CNT_BIT_RX_WRD_CNT_MSK, FC_CBR_REG_RX_WORD_CNT_BIT_RX_WRD_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_WRD_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc_cbr_field_RX_BAD_CRC_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_BAD_CRC_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_BAD_CRC_get", A, 12);
    /* (0x00000034 bits 31:0) field RX_BAD_CRC of register PMC_FC_CBR_REG_RX_FRAMES_BAD_CRC */
    reg_value = fc_cbr_reg_RX_FRAMES_BAD_CRC_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & FC_CBR_REG_RX_FRAMES_BAD_CRC_BIT_RX_BAD_CRC_MSK) >> FC_CBR_REG_RX_FRAMES_BAD_CRC_BIT_RX_BAD_CRC_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_BAD_CRC_get", A, value );

    return value;
}
static INLINE UINT32 fc_cbr_field_range_RX_BAD_CRC_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_RX_BAD_CRC_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_BAD_CRC_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_BAD_CRC_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_BAD_CRC_get", stop_bit, 31 );
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
    /* (0x00000034 bits 31:0) field RX_BAD_CRC of register PMC_FC_CBR_REG_RX_FRAMES_BAD_CRC */
    reg_value = fc_cbr_reg_RX_FRAMES_BAD_CRC_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & FC_CBR_REG_RX_FRAMES_BAD_CRC_BIT_RX_BAD_CRC_MSK)
                  >> FC_CBR_REG_RX_FRAMES_BAD_CRC_BIT_RX_BAD_CRC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_RX_FRAMES_BAD_CRC_BIT_RX_BAD_CRC_MSK, FC_CBR_REG_RX_FRAMES_BAD_CRC_BIT_RX_BAD_CRC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_BAD_CRC_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc_cbr_field_RX_TOT_PKTS_get( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_TOT_PKTS_get( fc_cbr_buffer_t *b_ptr,
                                                   fc800_pmg_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_TOT_PKTS_get", A, 12);
    /* (0x00000038 bits 31:0) field RX_TOT_PKTS of register PMC_FC_CBR_REG_RX_TOTAL_PACKETS */
    reg_value = fc_cbr_reg_RX_TOTAL_PACKETS_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_RX_TOTAL_PACKETS_BIT_RX_TOT_PKTS_MSK) >> FC_CBR_REG_RX_TOTAL_PACKETS_BIT_RX_TOT_PKTS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_TOT_PKTS_get", A, value );

    return value;
}
static INLINE UINT32 fc_cbr_field_range_RX_TOT_PKTS_get( fc_cbr_buffer_t *b_ptr,
                                                         fc800_pmg_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_RX_TOT_PKTS_get( fc_cbr_buffer_t *b_ptr,
                                                         fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_TOT_PKTS_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_TOT_PKTS_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_TOT_PKTS_get", stop_bit, 31 );
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
    /* (0x00000038 bits 31:0) field RX_TOT_PKTS of register PMC_FC_CBR_REG_RX_TOTAL_PACKETS */
    reg_value = fc_cbr_reg_RX_TOTAL_PACKETS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_RX_TOTAL_PACKETS_BIT_RX_TOT_PKTS_MSK)
                  >> FC_CBR_REG_RX_TOTAL_PACKETS_BIT_RX_TOT_PKTS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_RX_TOTAL_PACKETS_BIT_RX_TOT_PKTS_MSK, FC_CBR_REG_RX_TOTAL_PACKETS_BIT_RX_TOT_PKTS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_TOT_PKTS_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc_cbr_field_RX_LCV_CNT_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_RX_LCV_CNT_get( fc_cbr_buffer_t *b_ptr,
                                                  fc800_pmg_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_RX_LCV_CNT_get", A, 12);
    /* (0x00000090 bits 15:0) bits 0:15 use field RX_LCV_CNT of register PMC_FC_CBR_REG_LCV_CNT */
    reg_value = fc_cbr_reg_LCV_CNT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_REG_LCV_CNT_BIT_RX_LCV_CNT_MSK) >> FC_CBR_REG_LCV_CNT_BIT_RX_LCV_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_field_RX_LCV_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc_cbr_field_range_RX_LCV_CNT_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_field_range_RX_LCV_CNT_get( fc_cbr_buffer_t *b_ptr,
                                                        fc800_pmg_handle_t *h_ptr,
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

    if (A > 12)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_field_range_RX_LCV_CNT_get", A, 12);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_field_range_RX_LCV_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_field_range_RX_LCV_CNT_get", stop_bit, 15 );
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
    /* (0x00000090 bits 15:0) bits 0:15 use field RX_LCV_CNT of register PMC_FC_CBR_REG_LCV_CNT */
    reg_value = fc_cbr_reg_LCV_CNT_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_REG_LCV_CNT_BIT_RX_LCV_CNT_MSK)
                  >> FC_CBR_REG_LCV_CNT_BIT_RX_LCV_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_REG_LCV_CNT_BIT_RX_LCV_CNT_MSK, FC_CBR_REG_LCV_CNT_BIT_RX_LCV_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_field_range_RX_LCV_CNT_get", A, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC_CBR_IO_INLINE_H */
