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
 *     and register accessor functions for the l1tpp_fege block
 *****************************************************************************/
#ifndef _L1TPP_FEGE_IO_INLINE_H
#define _L1TPP_FEGE_IO_INLINE_H

#include "enet_fege_loc.h"
#include "l1tpp_fege_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define L1TPP_FEGE_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for l1tpp_fege
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
    enet_fege_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} l1tpp_fege_buffer_t;
static INLINE void l1tpp_fege_buffer_init( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_buffer_init( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "l1tpp_fege_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void l1tpp_fege_buffer_flush( l1tpp_fege_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_buffer_flush( l1tpp_fege_buffer_t *b_ptr )
{
    IOLOG( "l1tpp_fege_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 l1tpp_fege_reg_read( l1tpp_fege_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_reg_read( l1tpp_fege_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
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
static INLINE void l1tpp_fege_reg_write( l1tpp_fege_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_reg_write( l1tpp_fege_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
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

static INLINE void l1tpp_fege_field_set( l1tpp_fege_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 mask,
                                         UINT32 unused_mask,
                                         UINT32 ofs,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_set( l1tpp_fege_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
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

static INLINE void l1tpp_fege_action_on_write_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_action_on_write_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
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

static INLINE void l1tpp_fege_burst_read( l1tpp_fege_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_burst_read( l1tpp_fege_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
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

static INLINE void l1tpp_fege_burst_write( l1tpp_fege_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_burst_write( l1tpp_fege_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE l1tpp_fege_poll( l1tpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                    UINT32 max_count,
                                                    UINT32 *num_failed_polls,
                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1tpp_fege_poll( l1tpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
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
 *  register access functions for l1tpp_fege
 * ==================================================================================
 */

static INLINE void l1tpp_fege_reg_CFG_write( l1tpp_fege_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_reg_CFG_write( l1tpp_fege_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1tpp_fege_reg_CFG_write", value );
    l1tpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1TPP_FEGE_REG_CFG,
                          value);
}

static INLINE void l1tpp_fege_reg_CFG_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_reg_CFG_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1tpp_fege_reg_CFG_field_set", mask, ofs, value );
    l1tpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1TPP_FEGE_REG_CFG,
                          mask,
                          PMC_L1TPP_FEGE_REG_CFG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1tpp_fege_reg_CFG_read( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_reg_CFG_read( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1tpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1TPP_FEGE_REG_CFG);

    IOLOG( "%s -> 0x%08x;", "l1tpp_fege_reg_CFG_read", reg_value);
    return reg_value;
}

static INLINE void l1tpp_fege_reg_TENB_ERR_RDNEG_write( l1tpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_reg_TENB_ERR_RDNEG_write( l1tpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1tpp_fege_reg_TENB_ERR_RDNEG_write", value );
    l1tpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1TPP_FEGE_REG_TENB_ERR_RDNEG,
                          value);
}

static INLINE void l1tpp_fege_reg_TENB_ERR_RDNEG_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_reg_TENB_ERR_RDNEG_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1tpp_fege_reg_TENB_ERR_RDNEG_field_set", mask, ofs, value );
    l1tpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1TPP_FEGE_REG_TENB_ERR_RDNEG,
                          mask,
                          PMC_L1TPP_FEGE_REG_TENB_ERR_RDNEG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1tpp_fege_reg_TENB_ERR_RDNEG_read( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_reg_TENB_ERR_RDNEG_read( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1tpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1TPP_FEGE_REG_TENB_ERR_RDNEG);

    IOLOG( "%s -> 0x%08x;", "l1tpp_fege_reg_TENB_ERR_RDNEG_read", reg_value);
    return reg_value;
}

static INLINE void l1tpp_fege_reg_CSF_ERR_CODE_write( l1tpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_reg_CSF_ERR_CODE_write( l1tpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1tpp_fege_reg_CSF_ERR_CODE_write", value );
    l1tpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1TPP_FEGE_REG_CSF_ERR_CODE,
                          value);
}

static INLINE void l1tpp_fege_reg_CSF_ERR_CODE_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_reg_CSF_ERR_CODE_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1tpp_fege_reg_CSF_ERR_CODE_field_set", mask, ofs, value );
    l1tpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1TPP_FEGE_REG_CSF_ERR_CODE,
                          mask,
                          PMC_L1TPP_FEGE_REG_CSF_ERR_CODE_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1tpp_fege_reg_CSF_ERR_CODE_read( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_reg_CSF_ERR_CODE_read( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1tpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1TPP_FEGE_REG_CSF_ERR_CODE);

    IOLOG( "%s -> 0x%08x;", "l1tpp_fege_reg_CSF_ERR_CODE_read", reg_value);
    return reg_value;
}

static INLINE void l1tpp_fege_reg_JITTER_PATTERN_GENERATION_write( l1tpp_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_reg_JITTER_PATTERN_GENERATION_write( l1tpp_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1tpp_fege_reg_JITTER_PATTERN_GENERATION_write", value );
    l1tpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION,
                          value);
}

static INLINE void l1tpp_fege_reg_JITTER_PATTERN_GENERATION_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_reg_JITTER_PATTERN_GENERATION_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1tpp_fege_reg_JITTER_PATTERN_GENERATION_field_set", mask, ofs, value );
    l1tpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION,
                          mask,
                          PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1tpp_fege_reg_JITTER_PATTERN_GENERATION_read( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_reg_JITTER_PATTERN_GENERATION_read( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1tpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION);

    IOLOG( "%s -> 0x%08x;", "l1tpp_fege_reg_JITTER_PATTERN_GENERATION_read", reg_value);
    return reg_value;
}

static INLINE void l1tpp_fege_reg_CTRL_write( l1tpp_fege_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_reg_CTRL_write( l1tpp_fege_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1tpp_fege_reg_CTRL_write", value );
    l1tpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1TPP_FEGE_REG_CTRL,
                          value);
}

static INLINE void l1tpp_fege_reg_CTRL_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_reg_CTRL_field_set( l1tpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1tpp_fege_reg_CTRL_field_set", mask, ofs, value );
    l1tpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1TPP_FEGE_REG_CTRL,
                          mask,
                          PMC_L1TPP_FEGE_REG_CTRL_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1tpp_fege_reg_CTRL_read( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_reg_CTRL_read( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1tpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1TPP_FEGE_REG_CTRL);

    IOLOG( "%s -> 0x%08x;", "l1tpp_fege_reg_CTRL_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void l1tpp_fege_field_RD_ALIGN_EN_set( l1tpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_RD_ALIGN_EN_set( l1tpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1tpp_fege_field_RD_ALIGN_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1tpp_fege_field_RD_ALIGN_EN_set", value );

    /* (0x00002040 bits 8) field RD_ALIGN_EN of register PMC_L1TPP_FEGE_REG_CFG */
    l1tpp_fege_reg_CFG_field_set( b_ptr,
                                  h_ptr,
                                  L1TPP_FEGE_REG_CFG_BIT_RD_ALIGN_EN_MSK,
                                  L1TPP_FEGE_REG_CFG_BIT_RD_ALIGN_EN_OFF,
                                  value);
}

static INLINE UINT32 l1tpp_fege_field_RD_ALIGN_EN_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_RD_ALIGN_EN_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002040 bits 8) field RD_ALIGN_EN of register PMC_L1TPP_FEGE_REG_CFG */
    reg_value = l1tpp_fege_reg_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & L1TPP_FEGE_REG_CFG_BIT_RD_ALIGN_EN_MSK) >> L1TPP_FEGE_REG_CFG_BIT_RD_ALIGN_EN_OFF;
    IOLOG( "%s -> 0x%08x", "l1tpp_fege_field_RD_ALIGN_EN_get", value );

    return value;
}
static INLINE void l1tpp_fege_field_MODE_set( l1tpp_fege_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_MODE_set( l1tpp_fege_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1tpp_fege_field_MODE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "l1tpp_fege_field_MODE_set", value );

    /* (0x00002040 bits 3:1) bits 0:2 use field MODE of register PMC_L1TPP_FEGE_REG_CFG */
    l1tpp_fege_reg_CFG_field_set( b_ptr,
                                  h_ptr,
                                  L1TPP_FEGE_REG_CFG_BIT_MODE_MSK,
                                  L1TPP_FEGE_REG_CFG_BIT_MODE_OFF,
                                  value);
}

static INLINE UINT32 l1tpp_fege_field_MODE_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_MODE_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002040 bits 3:1) bits 0:2 use field MODE of register PMC_L1TPP_FEGE_REG_CFG */
    reg_value = l1tpp_fege_reg_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & L1TPP_FEGE_REG_CFG_BIT_MODE_MSK) >> L1TPP_FEGE_REG_CFG_BIT_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "l1tpp_fege_field_MODE_get", value );

    return value;
}
static INLINE void l1tpp_fege_field_range_MODE_set( l1tpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_range_MODE_set( l1tpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1tpp_fege_field_range_MODE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1tpp_fege_field_range_MODE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1tpp_fege_field_range_MODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002040 bits 3:1) bits 0:2 use field MODE of register PMC_L1TPP_FEGE_REG_CFG */
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
        /* (0x00002040 bits 3:1) bits 0:2 use field MODE of register PMC_L1TPP_FEGE_REG_CFG */
        l1tpp_fege_reg_CFG_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (L1TPP_FEGE_REG_CFG_BIT_MODE_OFF + subfield_offset),
                                      L1TPP_FEGE_REG_CFG_BIT_MODE_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 l1tpp_fege_field_range_MODE_get( l1tpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_range_MODE_get( l1tpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1tpp_fege_field_range_MODE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1tpp_fege_field_range_MODE_get", stop_bit, 2 );
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
    /* (0x00002040 bits 3:1) bits 0:2 use field MODE of register PMC_L1TPP_FEGE_REG_CFG */
    reg_value = l1tpp_fege_reg_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & L1TPP_FEGE_REG_CFG_BIT_MODE_MSK)
                  >> L1TPP_FEGE_REG_CFG_BIT_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1TPP_FEGE_REG_CFG_BIT_MODE_MSK, L1TPP_FEGE_REG_CFG_BIT_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1tpp_fege_field_range_MODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1tpp_fege_field_L1TPP_EN_set( l1tpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_L1TPP_EN_set( l1tpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1tpp_fege_field_L1TPP_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1tpp_fege_field_L1TPP_EN_set", value );

    /* (0x00002040 bits 0) field L1TPP_EN of register PMC_L1TPP_FEGE_REG_CFG */
    l1tpp_fege_reg_CFG_field_set( b_ptr,
                                  h_ptr,
                                  L1TPP_FEGE_REG_CFG_BIT_L1TPP_EN_MSK,
                                  L1TPP_FEGE_REG_CFG_BIT_L1TPP_EN_OFF,
                                  value);
}

static INLINE UINT32 l1tpp_fege_field_L1TPP_EN_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_L1TPP_EN_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002040 bits 0) field L1TPP_EN of register PMC_L1TPP_FEGE_REG_CFG */
    reg_value = l1tpp_fege_reg_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & L1TPP_FEGE_REG_CFG_BIT_L1TPP_EN_MSK) >> L1TPP_FEGE_REG_CFG_BIT_L1TPP_EN_OFF;
    IOLOG( "%s -> 0x%08x", "l1tpp_fege_field_L1TPP_EN_get", value );

    return value;
}
static INLINE void l1tpp_fege_field_TENB_ERR_RDNEG_set( l1tpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_TENB_ERR_RDNEG_set( l1tpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1tpp_fege_field_TENB_ERR_RDNEG_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "l1tpp_fege_field_TENB_ERR_RDNEG_set", value );

    /* (0x00002044 bits 9:0) bits 0:9 use field TENB_ERR_RDNEG of register PMC_L1TPP_FEGE_REG_TENB_ERR_RDNEG */
    l1tpp_fege_reg_TENB_ERR_RDNEG_field_set( b_ptr,
                                             h_ptr,
                                             L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_MSK,
                                             L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_OFF,
                                             value);
}

static INLINE UINT32 l1tpp_fege_field_TENB_ERR_RDNEG_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_TENB_ERR_RDNEG_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002044 bits 9:0) bits 0:9 use field TENB_ERR_RDNEG of register PMC_L1TPP_FEGE_REG_TENB_ERR_RDNEG */
    reg_value = l1tpp_fege_reg_TENB_ERR_RDNEG_read(  b_ptr, h_ptr);
    value = (reg_value & L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_MSK) >> L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_OFF;
    IOLOG( "%s -> 0x%08x", "l1tpp_fege_field_TENB_ERR_RDNEG_get", value );

    return value;
}
static INLINE void l1tpp_fege_field_range_TENB_ERR_RDNEG_set( l1tpp_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_range_TENB_ERR_RDNEG_set( l1tpp_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1tpp_fege_field_range_TENB_ERR_RDNEG_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1tpp_fege_field_range_TENB_ERR_RDNEG_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1tpp_fege_field_range_TENB_ERR_RDNEG_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002044 bits 9:0) bits 0:9 use field TENB_ERR_RDNEG of register PMC_L1TPP_FEGE_REG_TENB_ERR_RDNEG */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002044 bits 9:0) bits 0:9 use field TENB_ERR_RDNEG of register PMC_L1TPP_FEGE_REG_TENB_ERR_RDNEG */
        l1tpp_fege_reg_TENB_ERR_RDNEG_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_OFF + subfield_offset),
                                                 L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 l1tpp_fege_field_range_TENB_ERR_RDNEG_get( l1tpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_range_TENB_ERR_RDNEG_get( l1tpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1tpp_fege_field_range_TENB_ERR_RDNEG_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1tpp_fege_field_range_TENB_ERR_RDNEG_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00002044 bits 9:0) bits 0:9 use field TENB_ERR_RDNEG of register PMC_L1TPP_FEGE_REG_TENB_ERR_RDNEG */
    reg_value = l1tpp_fege_reg_TENB_ERR_RDNEG_read(  b_ptr, h_ptr);
    field_value = (reg_value & L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_MSK)
                  >> L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_MSK, L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1tpp_fege_field_range_TENB_ERR_RDNEG_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1tpp_fege_field_CSF_ERR_CODE_set( l1tpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_CSF_ERR_CODE_set( l1tpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1tpp_fege_field_CSF_ERR_CODE_set", value, 511);
    IOLOG( "%s <= 0x%08x", "l1tpp_fege_field_CSF_ERR_CODE_set", value );

    /* (0x00002048 bits 8:0) bits 0:8 use field CSF_ERR_CODE of register PMC_L1TPP_FEGE_REG_CSF_ERR_CODE */
    l1tpp_fege_reg_CSF_ERR_CODE_field_set( b_ptr,
                                           h_ptr,
                                           L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_MSK,
                                           L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_OFF,
                                           value);
}

static INLINE UINT32 l1tpp_fege_field_CSF_ERR_CODE_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_CSF_ERR_CODE_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002048 bits 8:0) bits 0:8 use field CSF_ERR_CODE of register PMC_L1TPP_FEGE_REG_CSF_ERR_CODE */
    reg_value = l1tpp_fege_reg_CSF_ERR_CODE_read(  b_ptr, h_ptr);
    value = (reg_value & L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_MSK) >> L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_OFF;
    IOLOG( "%s -> 0x%08x", "l1tpp_fege_field_CSF_ERR_CODE_get", value );

    return value;
}
static INLINE void l1tpp_fege_field_range_CSF_ERR_CODE_set( l1tpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_range_CSF_ERR_CODE_set( l1tpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1tpp_fege_field_range_CSF_ERR_CODE_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1tpp_fege_field_range_CSF_ERR_CODE_set", stop_bit, 8 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1tpp_fege_field_range_CSF_ERR_CODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002048 bits 8:0) bits 0:8 use field CSF_ERR_CODE of register PMC_L1TPP_FEGE_REG_CSF_ERR_CODE */
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
        /* (0x00002048 bits 8:0) bits 0:8 use field CSF_ERR_CODE of register PMC_L1TPP_FEGE_REG_CSF_ERR_CODE */
        l1tpp_fege_reg_CSF_ERR_CODE_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_OFF + subfield_offset),
                                               L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 l1tpp_fege_field_range_CSF_ERR_CODE_get( l1tpp_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_range_CSF_ERR_CODE_get( l1tpp_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1tpp_fege_field_range_CSF_ERR_CODE_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1tpp_fege_field_range_CSF_ERR_CODE_get", stop_bit, 8 );
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
    /* (0x00002048 bits 8:0) bits 0:8 use field CSF_ERR_CODE of register PMC_L1TPP_FEGE_REG_CSF_ERR_CODE */
    reg_value = l1tpp_fege_reg_CSF_ERR_CODE_read(  b_ptr, h_ptr);
    field_value = (reg_value & L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_MSK)
                  >> L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_MSK, L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1tpp_fege_field_range_CSF_ERR_CODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1tpp_fege_field_PTRN_SEL_set( l1tpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_PTRN_SEL_set( l1tpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1tpp_fege_field_PTRN_SEL_set", value, 7);
    IOLOG( "%s <= 0x%08x", "l1tpp_fege_field_PTRN_SEL_set", value );

    /* (0x0000204c bits 14:12) bits 0:2 use field PTRN_SEL of register PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION */
    l1tpp_fege_reg_JITTER_PATTERN_GENERATION_field_set( b_ptr,
                                                        h_ptr,
                                                        L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_MSK,
                                                        L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_OFF,
                                                        value);
}

static INLINE UINT32 l1tpp_fege_field_PTRN_SEL_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_PTRN_SEL_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000204c bits 14:12) bits 0:2 use field PTRN_SEL of register PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION */
    reg_value = l1tpp_fege_reg_JITTER_PATTERN_GENERATION_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_MSK) >> L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "l1tpp_fege_field_PTRN_SEL_get", value );

    return value;
}
static INLINE void l1tpp_fege_field_range_PTRN_SEL_set( l1tpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_range_PTRN_SEL_set( l1tpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1tpp_fege_field_range_PTRN_SEL_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1tpp_fege_field_range_PTRN_SEL_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1tpp_fege_field_range_PTRN_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000204c bits 14:12) bits 0:2 use field PTRN_SEL of register PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION */
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
        /* (0x0000204c bits 14:12) bits 0:2 use field PTRN_SEL of register PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION */
        l1tpp_fege_reg_JITTER_PATTERN_GENERATION_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_OFF + subfield_offset),
                                                            L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 l1tpp_fege_field_range_PTRN_SEL_get( l1tpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_range_PTRN_SEL_get( l1tpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1tpp_fege_field_range_PTRN_SEL_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1tpp_fege_field_range_PTRN_SEL_get", stop_bit, 2 );
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
    /* (0x0000204c bits 14:12) bits 0:2 use field PTRN_SEL of register PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION */
    reg_value = l1tpp_fege_reg_JITTER_PATTERN_GENERATION_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_MSK)
                  >> L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_MSK, L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1tpp_fege_field_range_PTRN_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1tpp_fege_field_CUS_PTRN_set( l1tpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_CUS_PTRN_set( l1tpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1tpp_fege_field_CUS_PTRN_set", value, 511);
    IOLOG( "%s <= 0x%08x", "l1tpp_fege_field_CUS_PTRN_set", value );

    /* (0x0000204c bits 8:0) bits 0:8 use field CUS_PTRN of register PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION */
    l1tpp_fege_reg_JITTER_PATTERN_GENERATION_field_set( b_ptr,
                                                        h_ptr,
                                                        L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_MSK,
                                                        L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_OFF,
                                                        value);
}

static INLINE UINT32 l1tpp_fege_field_CUS_PTRN_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_CUS_PTRN_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000204c bits 8:0) bits 0:8 use field CUS_PTRN of register PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION */
    reg_value = l1tpp_fege_reg_JITTER_PATTERN_GENERATION_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_MSK) >> L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_OFF;
    IOLOG( "%s -> 0x%08x", "l1tpp_fege_field_CUS_PTRN_get", value );

    return value;
}
static INLINE void l1tpp_fege_field_range_CUS_PTRN_set( l1tpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_range_CUS_PTRN_set( l1tpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1tpp_fege_field_range_CUS_PTRN_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1tpp_fege_field_range_CUS_PTRN_set", stop_bit, 8 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1tpp_fege_field_range_CUS_PTRN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000204c bits 8:0) bits 0:8 use field CUS_PTRN of register PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION */
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
        /* (0x0000204c bits 8:0) bits 0:8 use field CUS_PTRN of register PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION */
        l1tpp_fege_reg_JITTER_PATTERN_GENERATION_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_OFF + subfield_offset),
                                                            L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 l1tpp_fege_field_range_CUS_PTRN_get( l1tpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_range_CUS_PTRN_get( l1tpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1tpp_fege_field_range_CUS_PTRN_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1tpp_fege_field_range_CUS_PTRN_get", stop_bit, 8 );
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
    /* (0x0000204c bits 8:0) bits 0:8 use field CUS_PTRN of register PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION */
    reg_value = l1tpp_fege_reg_JITTER_PATTERN_GENERATION_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_MSK)
                  >> L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_MSK, L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1tpp_fege_field_range_CUS_PTRN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1tpp_fege_field_FEF_GEN_EN_set( l1tpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void l1tpp_fege_field_FEF_GEN_EN_set( l1tpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1tpp_fege_field_FEF_GEN_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1tpp_fege_field_FEF_GEN_EN_set", value );

    /* (0x00002054 bits 4) field FEF_GEN_EN of register PMC_L1TPP_FEGE_REG_CTRL */
    l1tpp_fege_reg_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   L1TPP_FEGE_REG_CTRL_BIT_FEF_GEN_EN_MSK,
                                   L1TPP_FEGE_REG_CTRL_BIT_FEF_GEN_EN_OFF,
                                   value);
}

static INLINE UINT32 l1tpp_fege_field_FEF_GEN_EN_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_FEF_GEN_EN_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002054 bits 4) field FEF_GEN_EN of register PMC_L1TPP_FEGE_REG_CTRL */
    reg_value = l1tpp_fege_reg_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & L1TPP_FEGE_REG_CTRL_BIT_FEF_GEN_EN_MSK) >> L1TPP_FEGE_REG_CTRL_BIT_FEF_GEN_EN_OFF;
    IOLOG( "%s -> 0x%08x", "l1tpp_fege_field_FEF_GEN_EN_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 l1tpp_fege_field_L1TPP_STATUS_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_L1TPP_STATUS_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002040 bits 15) field L1TPP_STATUS of register PMC_L1TPP_FEGE_REG_CFG */
    reg_value = l1tpp_fege_reg_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & L1TPP_FEGE_REG_CFG_BIT_L1TPP_STATUS_MSK) >> L1TPP_FEGE_REG_CFG_BIT_L1TPP_STATUS_OFF;
    IOLOG( "%s -> 0x%08x", "l1tpp_fege_field_L1TPP_STATUS_get", value );

    return value;
}
static INLINE UINT32 l1tpp_fege_field_INVLD_FRAME_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1tpp_fege_field_INVLD_FRAME_get( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002054 bits 0) field INVLD_FRAME of register PMC_L1TPP_FEGE_REG_CTRL */
    reg_value = l1tpp_fege_reg_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & L1TPP_FEGE_REG_CTRL_BIT_INVLD_FRAME_MSK) >> L1TPP_FEGE_REG_CTRL_BIT_INVLD_FRAME_OFF;
    IOLOG( "%s -> 0x%08x", "l1tpp_fege_field_INVLD_FRAME_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _L1TPP_FEGE_IO_INLINE_H */
