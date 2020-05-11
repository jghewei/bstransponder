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
 *     and register accessor functions for the otudg block
 *****************************************************************************/
#ifndef _OTUDG_IO_INLINE_H
#define _OTUDG_IO_INLINE_H

#include "pmc_sys.h"
#include "pmc_types.h"
#include "otudg_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OTUDG_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for otudg
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
} otudg_buffer_t;
static INLINE void otudg_buffer_init( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void otudg_buffer_init( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "otudg_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void otudg_buffer_flush( otudg_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void otudg_buffer_flush( otudg_buffer_t *b_ptr )
{
    IOLOG( "otudg_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 otudg_reg_read( otudg_buffer_t *b_ptr,
                                     otu_frm_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 otudg_reg_read( otudg_buffer_t *b_ptr,
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
static INLINE void otudg_reg_write( otudg_buffer_t *b_ptr,
                                    otu_frm_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_write( otudg_buffer_t *b_ptr,
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

static INLINE void otudg_field_set( otudg_buffer_t *b_ptr,
                                    otu_frm_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 mask,
                                    UINT32 unused_mask,
                                    UINT32 ofs,
                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_set( otudg_buffer_t *b_ptr,
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

static INLINE void otudg_action_on_write_field_set( otudg_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_action_on_write_field_set( otudg_buffer_t *b_ptr,
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

static INLINE void otudg_burst_read( otudg_buffer_t *b_ptr,
                                     otu_frm_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void otudg_burst_read( otudg_buffer_t *b_ptr,
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

static INLINE void otudg_burst_write( otudg_buffer_t *b_ptr,
                                      otu_frm_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void otudg_burst_write( otudg_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE otudg_poll( otudg_buffer_t *b_ptr,
                                               otu_frm_handle_t *h_ptr,
                                               UINT32 mem_type,
                                               UINT32 reg,
                                               UINT32 mask,
                                               UINT32 value,
                                               PMC_POLL_COMPARISON_TYPE cmp,
                                               UINT32 max_count,
                                               UINT32 *num_failed_polls,
                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otudg_poll( otudg_buffer_t *b_ptr,
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
 *  register access functions for otudg
 * ==================================================================================
 */

static INLINE void otudg_reg_FIFO_CTRL_REG_write( otudg_buffer_t *b_ptr,
                                                  otu_frm_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_FIFO_CTRL_REG_write( otudg_buffer_t *b_ptr,
                                                  otu_frm_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otudg_reg_FIFO_CTRL_REG_write", value );
    otudg_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OTUDG_REG_FIFO_CTRL_REG,
                     value);
}

static INLINE void otudg_reg_FIFO_CTRL_REG_field_set( otudg_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_FIFO_CTRL_REG_field_set( otudg_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otudg_reg_FIFO_CTRL_REG_field_set", mask, ofs, value );
    otudg_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OTUDG_REG_FIFO_CTRL_REG,
                     mask,
                     PMC_OTUDG_REG_FIFO_CTRL_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 otudg_reg_FIFO_CTRL_REG_read( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_reg_FIFO_CTRL_REG_read( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otudg_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OTUDG_REG_FIFO_CTRL_REG);

    IOLOG( "%s -> 0x%08x;", "otudg_reg_FIFO_CTRL_REG_read", reg_value);
    return reg_value;
}

static INLINE void otudg_reg_GEN_CFG_REG_array_write( otudg_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_GEN_CFG_REG_array_write( otudg_buffer_t *b_ptr,
                                                      otu_frm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otudg_reg_GEN_CFG_REG_array_write", value );
    otudg_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OTUDG_REG_GEN_CFG_REG(N),
                     value);
}

static INLINE void otudg_reg_GEN_CFG_REG_array_field_set( otudg_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_GEN_CFG_REG_array_field_set( otudg_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otudg_reg_GEN_CFG_REG_array_field_set", N, mask, ofs, value );
    otudg_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OTUDG_REG_GEN_CFG_REG(N),
                     mask,
                     PMC_OTUDG_REG_GEN_CFG_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 otudg_reg_GEN_CFG_REG_array_read( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_reg_GEN_CFG_REG_array_read( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = otudg_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OTUDG_REG_GEN_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otudg_reg_GEN_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otudg_reg_SLICE_DSM_NUM_REG_array_write( otudg_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_SLICE_DSM_NUM_REG_array_write( otudg_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otudg_reg_SLICE_DSM_NUM_REG_array_write", value );
    otudg_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OTUDG_REG_SLICE_DSM_NUM_REG(N),
                     value);
}

static INLINE void otudg_reg_SLICE_DSM_NUM_REG_array_field_set( otudg_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_SLICE_DSM_NUM_REG_array_field_set( otudg_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otudg_reg_SLICE_DSM_NUM_REG_array_field_set", N, mask, ofs, value );
    otudg_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OTUDG_REG_SLICE_DSM_NUM_REG(N),
                     mask,
                     PMC_OTUDG_REG_SLICE_DSM_NUM_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 otudg_reg_SLICE_DSM_NUM_REG_array_read( otudg_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_reg_SLICE_DSM_NUM_REG_array_read( otudg_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = otudg_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OTUDG_REG_SLICE_DSM_NUM_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otudg_reg_SLICE_DSM_NUM_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otudg_reg_SLICE_DSM_DEN_REG_array_write( otudg_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_SLICE_DSM_DEN_REG_array_write( otudg_buffer_t *b_ptr,
                                                            otu_frm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otudg_reg_SLICE_DSM_DEN_REG_array_write", value );
    otudg_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OTUDG_REG_SLICE_DSM_DEN_REG(N),
                     value);
}

static INLINE void otudg_reg_SLICE_DSM_DEN_REG_array_field_set( otudg_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_SLICE_DSM_DEN_REG_array_field_set( otudg_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otudg_reg_SLICE_DSM_DEN_REG_array_field_set", N, mask, ofs, value );
    otudg_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OTUDG_REG_SLICE_DSM_DEN_REG(N),
                     mask,
                     PMC_OTUDG_REG_SLICE_DSM_DEN_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 otudg_reg_SLICE_DSM_DEN_REG_array_read( otudg_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_reg_SLICE_DSM_DEN_REG_array_read( otudg_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = otudg_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OTUDG_REG_SLICE_DSM_DEN_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otudg_reg_SLICE_DSM_DEN_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otudg_reg_V_FIFO_OVR_INT_ENREG_write( otudg_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_V_FIFO_OVR_INT_ENREG_write( otudg_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otudg_reg_V_FIFO_OVR_INT_ENREG_write", value );
    otudg_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OTUDG_REG_V_FIFO_OVR_INT_ENREG,
                     value);
}

static INLINE void otudg_reg_V_FIFO_OVR_INT_ENREG_field_set( otudg_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_V_FIFO_OVR_INT_ENREG_field_set( otudg_buffer_t *b_ptr,
                                                             otu_frm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otudg_reg_V_FIFO_OVR_INT_ENREG_field_set", mask, ofs, value );
    otudg_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OTUDG_REG_V_FIFO_OVR_INT_ENREG,
                     mask,
                     PMC_OTUDG_REG_V_FIFO_OVR_INT_ENREG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 otudg_reg_V_FIFO_OVR_INT_ENREG_read( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_reg_V_FIFO_OVR_INT_ENREG_read( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otudg_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OTUDG_REG_V_FIFO_OVR_INT_ENREG);

    IOLOG( "%s -> 0x%08x;", "otudg_reg_V_FIFO_OVR_INT_ENREG_read", reg_value);
    return reg_value;
}

static INLINE void otudg_reg_V_FIFO_UDR_INT_EN_REG_write( otudg_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_V_FIFO_UDR_INT_EN_REG_write( otudg_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otudg_reg_V_FIFO_UDR_INT_EN_REG_write", value );
    otudg_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OTUDG_REG_V_FIFO_UDR_INT_EN_REG,
                     value);
}

static INLINE void otudg_reg_V_FIFO_UDR_INT_EN_REG_field_set( otudg_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_V_FIFO_UDR_INT_EN_REG_field_set( otudg_buffer_t *b_ptr,
                                                              otu_frm_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otudg_reg_V_FIFO_UDR_INT_EN_REG_field_set", mask, ofs, value );
    otudg_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OTUDG_REG_V_FIFO_UDR_INT_EN_REG,
                     mask,
                     PMC_OTUDG_REG_V_FIFO_UDR_INT_EN_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 otudg_reg_V_FIFO_UDR_INT_EN_REG_read( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_reg_V_FIFO_UDR_INT_EN_REG_read( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otudg_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OTUDG_REG_V_FIFO_UDR_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "otudg_reg_V_FIFO_UDR_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void otudg_reg_V_FIFO_OVR_INT_REG_write( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_V_FIFO_OVR_INT_REG_write( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otudg_reg_V_FIFO_OVR_INT_REG_write", value );
    otudg_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OTUDG_REG_V_FIFO_OVR_INT_REG,
                     value);
}

static INLINE void otudg_reg_V_FIFO_OVR_INT_REG_action_on_write_field_set( otudg_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_V_FIFO_OVR_INT_REG_action_on_write_field_set( otudg_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otudg_reg_V_FIFO_OVR_INT_REG_action_on_write_field_set", mask, ofs, value );
    otudg_action_on_write_field_set( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTUDG_REG_V_FIFO_OVR_INT_REG,
                                     mask,
                                     ofs,
                                     value);

}

static INLINE UINT32 otudg_reg_V_FIFO_OVR_INT_REG_read( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_reg_V_FIFO_OVR_INT_REG_read( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otudg_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OTUDG_REG_V_FIFO_OVR_INT_REG);

    IOLOG( "%s -> 0x%08x;", "otudg_reg_V_FIFO_OVR_INT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otudg_reg_V_FIFO_OVR_INT_REG_poll( otudg_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otudg_reg_V_FIFO_OVR_INT_REG_poll( otudg_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otudg_reg_V_FIFO_OVR_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otudg_poll( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTUDG_REG_V_FIFO_OVR_INT_REG,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE void otudg_reg_V_FIFO_UDR_INT_REG_write( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_V_FIFO_UDR_INT_REG_write( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otudg_reg_V_FIFO_UDR_INT_REG_write", value );
    otudg_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OTUDG_REG_V_FIFO_UDR_INT_REG,
                     value);
}

static INLINE void otudg_reg_V_FIFO_UDR_INT_REG_action_on_write_field_set( otudg_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_reg_V_FIFO_UDR_INT_REG_action_on_write_field_set( otudg_buffer_t *b_ptr,
                                                                           otu_frm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otudg_reg_V_FIFO_UDR_INT_REG_action_on_write_field_set", mask, ofs, value );
    otudg_action_on_write_field_set( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OTUDG_REG_V_FIFO_UDR_INT_REG,
                                     mask,
                                     ofs,
                                     value);

}

static INLINE UINT32 otudg_reg_V_FIFO_UDR_INT_REG_read( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_reg_V_FIFO_UDR_INT_REG_read( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otudg_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OTUDG_REG_V_FIFO_UDR_INT_REG);

    IOLOG( "%s -> 0x%08x;", "otudg_reg_V_FIFO_UDR_INT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otudg_reg_V_FIFO_UDR_INT_REG_poll( otudg_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otudg_reg_V_FIFO_UDR_INT_REG_poll( otudg_buffer_t *b_ptr,
                                                                      otu_frm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otudg_reg_V_FIFO_UDR_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otudg_poll( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTUDG_REG_V_FIFO_UDR_INT_REG,
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
static INLINE void otudg_field_CENTER_THRES_set( otudg_buffer_t *b_ptr,
                                                 otu_frm_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_CENTER_THRES_set( otudg_buffer_t *b_ptr,
                                                 otu_frm_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otudg_field_CENTER_THRES_set", value, 255);
    IOLOG( "%s <= 0x%08x", "otudg_field_CENTER_THRES_set", value );

    /* (0x00000400 bits 23:16) field CENTER_THRES of register PMC_OTUDG_REG_FIFO_CTRL_REG */
    otudg_reg_FIFO_CTRL_REG_field_set( b_ptr,
                                       h_ptr,
                                       OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_MSK,
                                       OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_OFF,
                                       value);
}

static INLINE UINT32 otudg_field_CENTER_THRES_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_CENTER_THRES_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000400 bits 23:16) field CENTER_THRES of register PMC_OTUDG_REG_FIFO_CTRL_REG */
    reg_value = otudg_reg_FIFO_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_MSK) >> OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_OFF;
    IOLOG( "%s -> 0x%08x", "otudg_field_CENTER_THRES_get", value );

    return value;
}
static INLINE void otudg_field_range_CENTER_THRES_set( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_range_CENTER_THRES_set( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_CENTER_THRES_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_CENTER_THRES_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_CENTER_THRES_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000400 bits 23:16) field CENTER_THRES of register PMC_OTUDG_REG_FIFO_CTRL_REG */
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
        /* (0x00000400 bits 23:16) field CENTER_THRES of register PMC_OTUDG_REG_FIFO_CTRL_REG */
        otudg_reg_FIFO_CTRL_REG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_OFF + subfield_offset),
                                           OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otudg_field_range_CENTER_THRES_get( otudg_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_range_CENTER_THRES_get( otudg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_CENTER_THRES_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_CENTER_THRES_get", stop_bit, 7 );
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
    /* (0x00000400 bits 23:16) field CENTER_THRES of register PMC_OTUDG_REG_FIFO_CTRL_REG */
    reg_value = otudg_reg_FIFO_CTRL_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_MSK)
                  >> OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_MSK, OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_CENTER_THRES_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otudg_field_A_FULL_THRES_set( otudg_buffer_t *b_ptr,
                                                 otu_frm_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_A_FULL_THRES_set( otudg_buffer_t *b_ptr,
                                                 otu_frm_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otudg_field_A_FULL_THRES_set", value, 255);
    IOLOG( "%s <= 0x%08x", "otudg_field_A_FULL_THRES_set", value );

    /* (0x00000400 bits 7:0) field A_FULL_THRES of register PMC_OTUDG_REG_FIFO_CTRL_REG */
    otudg_reg_FIFO_CTRL_REG_field_set( b_ptr,
                                       h_ptr,
                                       OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_MSK,
                                       OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_OFF,
                                       value);
}

static INLINE UINT32 otudg_field_A_FULL_THRES_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_A_FULL_THRES_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000400 bits 7:0) field A_FULL_THRES of register PMC_OTUDG_REG_FIFO_CTRL_REG */
    reg_value = otudg_reg_FIFO_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_MSK) >> OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_OFF;
    IOLOG( "%s -> 0x%08x", "otudg_field_A_FULL_THRES_get", value );

    return value;
}
static INLINE void otudg_field_range_A_FULL_THRES_set( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_range_A_FULL_THRES_set( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_A_FULL_THRES_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_A_FULL_THRES_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_A_FULL_THRES_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000400 bits 7:0) field A_FULL_THRES of register PMC_OTUDG_REG_FIFO_CTRL_REG */
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
        /* (0x00000400 bits 7:0) field A_FULL_THRES of register PMC_OTUDG_REG_FIFO_CTRL_REG */
        otudg_reg_FIFO_CTRL_REG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_OFF + subfield_offset),
                                           OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otudg_field_range_A_FULL_THRES_get( otudg_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_range_A_FULL_THRES_get( otudg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_A_FULL_THRES_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_A_FULL_THRES_get", stop_bit, 7 );
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
    /* (0x00000400 bits 7:0) field A_FULL_THRES of register PMC_OTUDG_REG_FIFO_CTRL_REG */
    reg_value = otudg_reg_FIFO_CTRL_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_MSK)
                  >> OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_MSK, OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_A_FULL_THRES_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void otudg_field_SLICE_EN_set( otudg_buffer_t *b_ptr,
                                             otu_frm_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_SLICE_EN_set( otudg_buffer_t *b_ptr,
                                             otu_frm_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otudg_field_SLICE_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otudg_field_SLICE_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otudg_field_SLICE_EN_set", N, value );

    /* ((0x00000404 + (N) * 0x10) bits 0) field SLICE_EN of register PMC_OTUDG_REG_GEN_CFG_REG index N=0..11 */
    otudg_reg_GEN_CFG_REG_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           OTUDG_REG_GEN_CFG_REG_BIT_SLICE_EN_MSK,
                                           OTUDG_REG_GEN_CFG_REG_BIT_SLICE_EN_OFF,
                                           value);
}

static INLINE UINT32 otudg_field_SLICE_EN_get( otudg_buffer_t *b_ptr,
                                               otu_frm_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_SLICE_EN_get( otudg_buffer_t *b_ptr,
                                               otu_frm_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otudg_field_SLICE_EN_get", N, 11);
    /* ((0x00000404 + (N) * 0x10) bits 0) field SLICE_EN of register PMC_OTUDG_REG_GEN_CFG_REG index N=0..11 */
    reg_value = otudg_reg_GEN_CFG_REG_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & OTUDG_REG_GEN_CFG_REG_BIT_SLICE_EN_MSK) >> OTUDG_REG_GEN_CFG_REG_BIT_SLICE_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otudg_field_SLICE_EN_get", N, value );

    return value;
}
static INLINE void otudg_field_DSM_NUM_set( otudg_buffer_t *b_ptr,
                                            otu_frm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_DSM_NUM_set( otudg_buffer_t *b_ptr,
                                            otu_frm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otudg_field_DSM_NUM_set", N, 11);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otudg_field_DSM_NUM_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "otudg_field_DSM_NUM_set", N, value );

    /* ((0x00000408 + (N) * 0x10) bits 23:0) field DSM_NUM of register PMC_OTUDG_REG_SLICE_DSM_NUM_REG index N=0..11 */
    otudg_reg_SLICE_DSM_NUM_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_MSK,
                                                 OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_OFF,
                                                 value);
}

static INLINE UINT32 otudg_field_DSM_NUM_get( otudg_buffer_t *b_ptr,
                                              otu_frm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_DSM_NUM_get( otudg_buffer_t *b_ptr,
                                              otu_frm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otudg_field_DSM_NUM_get", N, 11);
    /* ((0x00000408 + (N) * 0x10) bits 23:0) field DSM_NUM of register PMC_OTUDG_REG_SLICE_DSM_NUM_REG index N=0..11 */
    reg_value = otudg_reg_SLICE_DSM_NUM_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_MSK) >> OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otudg_field_DSM_NUM_get", N, value );

    return value;
}
static INLINE void otudg_field_range_DSM_NUM_set( otudg_buffer_t *b_ptr,
                                                  otu_frm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_range_DSM_NUM_set( otudg_buffer_t *b_ptr,
                                                  otu_frm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otudg_field_range_DSM_NUM_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_DSM_NUM_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_DSM_NUM_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_DSM_NUM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000408 + (N) * 0x10) bits 23:0) field DSM_NUM of register PMC_OTUDG_REG_SLICE_DSM_NUM_REG index N=0..11 */
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
        /* ((0x00000408 + (N) * 0x10) bits 23:0) field DSM_NUM of register PMC_OTUDG_REG_SLICE_DSM_NUM_REG index N=0..11 */
        otudg_reg_SLICE_DSM_NUM_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_OFF + subfield_offset),
                                                     OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 otudg_field_range_DSM_NUM_get( otudg_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_range_DSM_NUM_get( otudg_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otudg_field_range_DSM_NUM_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_DSM_NUM_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_DSM_NUM_get", stop_bit, 23 );
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
    /* ((0x00000408 + (N) * 0x10) bits 23:0) field DSM_NUM of register PMC_OTUDG_REG_SLICE_DSM_NUM_REG index N=0..11 */
    reg_value = otudg_reg_SLICE_DSM_NUM_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_MSK)
                  >> OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_MSK, OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_DSM_NUM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otudg_field_DSM_DEN_set( otudg_buffer_t *b_ptr,
                                            otu_frm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_DSM_DEN_set( otudg_buffer_t *b_ptr,
                                            otu_frm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otudg_field_DSM_DEN_set", N, 11);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otudg_field_DSM_DEN_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "otudg_field_DSM_DEN_set", N, value );

    /* ((0x0000040c + (N) * 0x10) bits 23:0) field DSM_DEN of register PMC_OTUDG_REG_SLICE_DSM_DEN_REG index N=0..11 */
    otudg_reg_SLICE_DSM_DEN_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_MSK,
                                                 OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_OFF,
                                                 value);
}

static INLINE UINT32 otudg_field_DSM_DEN_get( otudg_buffer_t *b_ptr,
                                              otu_frm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_DSM_DEN_get( otudg_buffer_t *b_ptr,
                                              otu_frm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otudg_field_DSM_DEN_get", N, 11);
    /* ((0x0000040c + (N) * 0x10) bits 23:0) field DSM_DEN of register PMC_OTUDG_REG_SLICE_DSM_DEN_REG index N=0..11 */
    reg_value = otudg_reg_SLICE_DSM_DEN_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_MSK) >> OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otudg_field_DSM_DEN_get", N, value );

    return value;
}
static INLINE void otudg_field_range_DSM_DEN_set( otudg_buffer_t *b_ptr,
                                                  otu_frm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_range_DSM_DEN_set( otudg_buffer_t *b_ptr,
                                                  otu_frm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otudg_field_range_DSM_DEN_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_DSM_DEN_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_DSM_DEN_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_DSM_DEN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000040c + (N) * 0x10) bits 23:0) field DSM_DEN of register PMC_OTUDG_REG_SLICE_DSM_DEN_REG index N=0..11 */
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
        /* ((0x0000040c + (N) * 0x10) bits 23:0) field DSM_DEN of register PMC_OTUDG_REG_SLICE_DSM_DEN_REG index N=0..11 */
        otudg_reg_SLICE_DSM_DEN_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_OFF + subfield_offset),
                                                     OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 otudg_field_range_DSM_DEN_get( otudg_buffer_t *b_ptr,
                                                    otu_frm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_range_DSM_DEN_get( otudg_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otudg_field_range_DSM_DEN_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_DSM_DEN_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_DSM_DEN_get", stop_bit, 23 );
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
    /* ((0x0000040c + (N) * 0x10) bits 23:0) field DSM_DEN of register PMC_OTUDG_REG_SLICE_DSM_DEN_REG index N=0..11 */
    reg_value = otudg_reg_SLICE_DSM_DEN_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_MSK)
                  >> OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_MSK, OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_DSM_DEN_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void otudg_field_V_FIFO_OVR_E_set( otudg_buffer_t *b_ptr,
                                                 otu_frm_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_V_FIFO_OVR_E_set( otudg_buffer_t *b_ptr,
                                                 otu_frm_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otudg_field_V_FIFO_OVR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otudg_field_V_FIFO_OVR_E_set", value );

    /* (0x000004f8 bits 11:0) bits 0:11 use field V_FIFO_OVR_E of register PMC_OTUDG_REG_V_FIFO_OVR_INT_ENREG */
    otudg_reg_V_FIFO_OVR_INT_ENREG_field_set( b_ptr,
                                              h_ptr,
                                              OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_MSK,
                                              OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_OFF,
                                              value);
}

static INLINE UINT32 otudg_field_V_FIFO_OVR_E_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_V_FIFO_OVR_E_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004f8 bits 11:0) bits 0:11 use field V_FIFO_OVR_E of register PMC_OTUDG_REG_V_FIFO_OVR_INT_ENREG */
    reg_value = otudg_reg_V_FIFO_OVR_INT_ENREG_read(  b_ptr, h_ptr);
    value = (reg_value & OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_MSK) >> OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "otudg_field_V_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void otudg_field_range_V_FIFO_OVR_E_set( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_range_V_FIFO_OVR_E_set( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_V_FIFO_OVR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_V_FIFO_OVR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_V_FIFO_OVR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000004f8 bits 11:0) bits 0:11 use field V_FIFO_OVR_E of register PMC_OTUDG_REG_V_FIFO_OVR_INT_ENREG */
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
        /* (0x000004f8 bits 11:0) bits 0:11 use field V_FIFO_OVR_E of register PMC_OTUDG_REG_V_FIFO_OVR_INT_ENREG */
        otudg_reg_V_FIFO_OVR_INT_ENREG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_OFF + subfield_offset),
                                                  OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 otudg_field_range_V_FIFO_OVR_E_get( otudg_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_range_V_FIFO_OVR_E_get( otudg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_V_FIFO_OVR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_V_FIFO_OVR_E_get", stop_bit, 11 );
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
    /* (0x000004f8 bits 11:0) bits 0:11 use field V_FIFO_OVR_E of register PMC_OTUDG_REG_V_FIFO_OVR_INT_ENREG */
    reg_value = otudg_reg_V_FIFO_OVR_INT_ENREG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_MSK)
                  >> OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_MSK, OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_V_FIFO_OVR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otudg_field_V_FIFO_UDR_E_set( otudg_buffer_t *b_ptr,
                                                 otu_frm_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_V_FIFO_UDR_E_set( otudg_buffer_t *b_ptr,
                                                 otu_frm_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otudg_field_V_FIFO_UDR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "otudg_field_V_FIFO_UDR_E_set", value );

    /* (0x000004fc bits 11:0) bits 0:11 use field V_FIFO_UDR_E of register PMC_OTUDG_REG_V_FIFO_UDR_INT_EN_REG */
    otudg_reg_V_FIFO_UDR_INT_EN_REG_field_set( b_ptr,
                                               h_ptr,
                                               OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_MSK,
                                               OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_OFF,
                                               value);
}

static INLINE UINT32 otudg_field_V_FIFO_UDR_E_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_V_FIFO_UDR_E_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004fc bits 11:0) bits 0:11 use field V_FIFO_UDR_E of register PMC_OTUDG_REG_V_FIFO_UDR_INT_EN_REG */
    reg_value = otudg_reg_V_FIFO_UDR_INT_EN_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_MSK) >> OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "otudg_field_V_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void otudg_field_range_V_FIFO_UDR_E_set( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_range_V_FIFO_UDR_E_set( otudg_buffer_t *b_ptr,
                                                       otu_frm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_V_FIFO_UDR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_V_FIFO_UDR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_V_FIFO_UDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000004fc bits 11:0) bits 0:11 use field V_FIFO_UDR_E of register PMC_OTUDG_REG_V_FIFO_UDR_INT_EN_REG */
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
        /* (0x000004fc bits 11:0) bits 0:11 use field V_FIFO_UDR_E of register PMC_OTUDG_REG_V_FIFO_UDR_INT_EN_REG */
        otudg_reg_V_FIFO_UDR_INT_EN_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_OFF + subfield_offset),
                                                   OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 otudg_field_range_V_FIFO_UDR_E_get( otudg_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_range_V_FIFO_UDR_E_get( otudg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_V_FIFO_UDR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_V_FIFO_UDR_E_get", stop_bit, 11 );
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
    /* (0x000004fc bits 11:0) bits 0:11 use field V_FIFO_UDR_E of register PMC_OTUDG_REG_V_FIFO_UDR_INT_EN_REG */
    reg_value = otudg_reg_V_FIFO_UDR_INT_EN_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_MSK)
                  >> OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_MSK, OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_V_FIFO_UDR_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void otudg_field_V_FIFO_OVR_I_set_to_clear( otudg_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_V_FIFO_OVR_I_set_to_clear( otudg_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otudg_field_V_FIFO_OVR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otudg_field_V_FIFO_OVR_I_set_to_clear", value );

    /* (0x000004f0 bits 11:0) bits 0:11 use field V_FIFO_OVR_I of register PMC_OTUDG_REG_V_FIFO_OVR_INT_REG */
    otudg_reg_V_FIFO_OVR_INT_REG_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_MSK,
                                                            OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_OFF,
                                                            value);
}

static INLINE UINT32 otudg_field_V_FIFO_OVR_I_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_V_FIFO_OVR_I_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004f0 bits 11:0) bits 0:11 use field V_FIFO_OVR_I of register PMC_OTUDG_REG_V_FIFO_OVR_INT_REG */
    reg_value = otudg_reg_V_FIFO_OVR_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_MSK) >> OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "otudg_field_V_FIFO_OVR_I_get", value );

    return value;
}
static INLINE void otudg_field_range_V_FIFO_OVR_I_set_to_clear( otudg_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_range_V_FIFO_OVR_I_set_to_clear( otudg_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_V_FIFO_OVR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_V_FIFO_OVR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_V_FIFO_OVR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000004f0 bits 11:0) bits 0:11 use field V_FIFO_OVR_I of register PMC_OTUDG_REG_V_FIFO_OVR_INT_REG */
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
        /* (0x000004f0 bits 11:0) bits 0:11 use field V_FIFO_OVR_I of register PMC_OTUDG_REG_V_FIFO_OVR_INT_REG */
        otudg_reg_V_FIFO_OVR_INT_REG_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_OFF + subfield_offset),
                                                                OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otudg_field_range_V_FIFO_OVR_I_get( otudg_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_range_V_FIFO_OVR_I_get( otudg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_V_FIFO_OVR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_V_FIFO_OVR_I_get", stop_bit, 11 );
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
    /* (0x000004f0 bits 11:0) bits 0:11 use field V_FIFO_OVR_I of register PMC_OTUDG_REG_V_FIFO_OVR_INT_REG */
    reg_value = otudg_reg_V_FIFO_OVR_INT_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_MSK)
                  >> OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_MSK, OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_V_FIFO_OVR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otudg_field_range_V_FIFO_OVR_I_poll( otudg_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otudg_field_range_V_FIFO_OVR_I_poll( otudg_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_V_FIFO_OVR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_V_FIFO_OVR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_V_FIFO_OVR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000004f0 bits 11:0) bits 0:11 use field V_FIFO_OVR_I of register PMC_OTUDG_REG_V_FIFO_OVR_INT_REG */
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
        /* (0x000004f0 bits 11:0) bits 0:11 use field V_FIFO_OVR_I of register PMC_OTUDG_REG_V_FIFO_OVR_INT_REG */
        return otudg_reg_V_FIFO_OVR_INT_REG_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_OFF + subfield_offset),
                                                  value << (OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otudg_field_V_FIFO_OVR_I_poll( otudg_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otudg_field_V_FIFO_OVR_I_poll( otudg_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otudg_field_V_FIFO_OVR_I_poll", value );

    /* (0x000004f0 bits 11:0) bits 0:11 use field V_FIFO_OVR_I of register PMC_OTUDG_REG_V_FIFO_OVR_INT_REG */
    return otudg_reg_V_FIFO_OVR_INT_REG_poll( b_ptr,
                                              h_ptr,
                                              OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_MSK,
                                              (value<<OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void otudg_field_V_FIFO_UDR_I_set_to_clear( otudg_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_V_FIFO_UDR_I_set_to_clear( otudg_buffer_t *b_ptr,
                                                          otu_frm_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otudg_field_V_FIFO_UDR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "otudg_field_V_FIFO_UDR_I_set_to_clear", value );

    /* (0x000004f4 bits 11:0) bits 0:11 use field V_FIFO_UDR_I of register PMC_OTUDG_REG_V_FIFO_UDR_INT_REG */
    otudg_reg_V_FIFO_UDR_INT_REG_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_MSK,
                                                            OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_OFF,
                                                            value);
}

static INLINE UINT32 otudg_field_V_FIFO_UDR_I_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_V_FIFO_UDR_I_get( otudg_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004f4 bits 11:0) bits 0:11 use field V_FIFO_UDR_I of register PMC_OTUDG_REG_V_FIFO_UDR_INT_REG */
    reg_value = otudg_reg_V_FIFO_UDR_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_MSK) >> OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "otudg_field_V_FIFO_UDR_I_get", value );

    return value;
}
static INLINE void otudg_field_range_V_FIFO_UDR_I_set_to_clear( otudg_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otudg_field_range_V_FIFO_UDR_I_set_to_clear( otudg_buffer_t *b_ptr,
                                                                otu_frm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_V_FIFO_UDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_V_FIFO_UDR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_V_FIFO_UDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000004f4 bits 11:0) bits 0:11 use field V_FIFO_UDR_I of register PMC_OTUDG_REG_V_FIFO_UDR_INT_REG */
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
        /* (0x000004f4 bits 11:0) bits 0:11 use field V_FIFO_UDR_I of register PMC_OTUDG_REG_V_FIFO_UDR_INT_REG */
        otudg_reg_V_FIFO_UDR_INT_REG_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_OFF + subfield_offset),
                                                                OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otudg_field_range_V_FIFO_UDR_I_get( otudg_buffer_t *b_ptr,
                                                         otu_frm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otudg_field_range_V_FIFO_UDR_I_get( otudg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_V_FIFO_UDR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_V_FIFO_UDR_I_get", stop_bit, 11 );
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
    /* (0x000004f4 bits 11:0) bits 0:11 use field V_FIFO_UDR_I of register PMC_OTUDG_REG_V_FIFO_UDR_INT_REG */
    reg_value = otudg_reg_V_FIFO_UDR_INT_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_MSK)
                  >> OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_MSK, OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_V_FIFO_UDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otudg_field_range_V_FIFO_UDR_I_poll( otudg_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otudg_field_range_V_FIFO_UDR_I_poll( otudg_buffer_t *b_ptr,
                                                                        otu_frm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otudg_field_range_V_FIFO_UDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otudg_field_range_V_FIFO_UDR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otudg_field_range_V_FIFO_UDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000004f4 bits 11:0) bits 0:11 use field V_FIFO_UDR_I of register PMC_OTUDG_REG_V_FIFO_UDR_INT_REG */
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
        /* (0x000004f4 bits 11:0) bits 0:11 use field V_FIFO_UDR_I of register PMC_OTUDG_REG_V_FIFO_UDR_INT_REG */
        return otudg_reg_V_FIFO_UDR_INT_REG_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_OFF + subfield_offset),
                                                  value << (OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otudg_field_V_FIFO_UDR_I_poll( otudg_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otudg_field_V_FIFO_UDR_I_poll( otudg_buffer_t *b_ptr,
                                                                  otu_frm_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otudg_field_V_FIFO_UDR_I_poll", value );

    /* (0x000004f4 bits 11:0) bits 0:11 use field V_FIFO_UDR_I of register PMC_OTUDG_REG_V_FIFO_UDR_INT_REG */
    return otudg_reg_V_FIFO_UDR_INT_REG_poll( b_ptr,
                                              h_ptr,
                                              OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_MSK,
                                              (value<<OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OTUDG_IO_INLINE_H */
