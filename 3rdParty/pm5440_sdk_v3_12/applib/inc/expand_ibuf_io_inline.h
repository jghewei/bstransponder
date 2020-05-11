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
 *     and register accessor functions for the expand_ibuf block
 *****************************************************************************/
#ifndef _EXPAND_IBUF_IO_INLINE_H
#define _EXPAND_IBUF_IO_INLINE_H

#include "expand_ibuf_loc.h"
#include "expand_ibuf_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EXPAND_IBUF_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for expand_ibuf
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
    expand_ibuf_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} expand_ibuf_buffer_t;
static INLINE void expand_ibuf_buffer_init( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void expand_ibuf_buffer_init( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "expand_ibuf_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void expand_ibuf_buffer_flush( expand_ibuf_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void expand_ibuf_buffer_flush( expand_ibuf_buffer_t *b_ptr )
{
    IOLOG( "expand_ibuf_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 expand_ibuf_reg_read( expand_ibuf_buffer_t *b_ptr,
                                           expand_ibuf_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_reg_read( expand_ibuf_buffer_t *b_ptr,
                                           expand_ibuf_handle_t *h_ptr,
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
static INLINE void expand_ibuf_reg_write( expand_ibuf_buffer_t *b_ptr,
                                          expand_ibuf_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_write( expand_ibuf_buffer_t *b_ptr,
                                          expand_ibuf_handle_t *h_ptr,
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

static INLINE void expand_ibuf_field_set( expand_ibuf_buffer_t *b_ptr,
                                          expand_ibuf_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 mask,
                                          UINT32 unused_mask,
                                          UINT32 ofs,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_set( expand_ibuf_buffer_t *b_ptr,
                                          expand_ibuf_handle_t *h_ptr,
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

static INLINE void expand_ibuf_action_on_write_field_set( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32 mem_type,
                                                          UINT32 reg,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_action_on_write_field_set( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
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

static INLINE void expand_ibuf_burst_read( expand_ibuf_buffer_t *b_ptr,
                                           expand_ibuf_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_burst_read( expand_ibuf_buffer_t *b_ptr,
                                           expand_ibuf_handle_t *h_ptr,
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

static INLINE void expand_ibuf_burst_write( expand_ibuf_buffer_t *b_ptr,
                                            expand_ibuf_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_burst_write( expand_ibuf_buffer_t *b_ptr,
                                            expand_ibuf_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE expand_ibuf_poll( expand_ibuf_buffer_t *b_ptr,
                                                     expand_ibuf_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 value,
                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                     UINT32 max_count,
                                                     UINT32 *num_failed_polls,
                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE expand_ibuf_poll( expand_ibuf_buffer_t *b_ptr,
                                                     expand_ibuf_handle_t *h_ptr,
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
 *  register access functions for expand_ibuf
 * ==================================================================================
 */

static INLINE void expand_ibuf_reg_MEM_CFG_STA_ID_write( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_MEM_CFG_STA_ID_write( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "expand_ibuf_reg_MEM_CFG_STA_ID_write", value );
    expand_ibuf_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID,
                           value);
}

static INLINE void expand_ibuf_reg_MEM_CFG_STA_ID_field_set( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_MEM_CFG_STA_ID_field_set( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "expand_ibuf_reg_MEM_CFG_STA_ID_field_set", mask, ofs, value );
    expand_ibuf_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID,
                           mask,
                           PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 expand_ibuf_reg_MEM_CFG_STA_ID_read( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_reg_MEM_CFG_STA_ID_read( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = expand_ibuf_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID);

    IOLOG( "%s -> 0x%08x;", "expand_ibuf_reg_MEM_CFG_STA_ID_read", reg_value);
    return reg_value;
}

static INLINE void expand_ibuf_reg_CFG_UPDATE_array_write( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_CFG_UPDATE_array_write( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "expand_ibuf_reg_CFG_UPDATE_array_write", value );
    expand_ibuf_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_EXPAND_IBUF_REG_CFG_UPDATE(N),
                           value);
}

static INLINE void expand_ibuf_reg_CFG_UPDATE_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_CFG_UPDATE_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "expand_ibuf_reg_CFG_UPDATE_array_field_set", N, mask, ofs, value );
    expand_ibuf_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_EXPAND_IBUF_REG_CFG_UPDATE(N),
                           mask,
                           PMC_EXPAND_IBUF_REG_CFG_UPDATE_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 expand_ibuf_reg_CFG_UPDATE_array_read( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_reg_CFG_UPDATE_array_read( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = expand_ibuf_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_EXPAND_IBUF_REG_CFG_UPDATE(N));

    IOLOG( "%s -> 0x%08x; N=%d", "expand_ibuf_reg_CFG_UPDATE_array_read", reg_value, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE expand_ibuf_reg_CFG_UPDATE_array_poll( expand_ibuf_buffer_t *b_ptr,
                                                                          expand_ibuf_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE expand_ibuf_reg_CFG_UPDATE_array_poll( expand_ibuf_buffer_t *b_ptr,
                                                                          expand_ibuf_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "expand_ibuf_reg_CFG_UPDATE_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return expand_ibuf_poll( b_ptr,
                             h_ptr,
                             MEM_TYPE_CONFIG,
                             PMC_EXPAND_IBUF_REG_CFG_UPDATE(N),
                             mask,
                             value,
                             cmp,
                             max_count,
                             num_failed_polls,
                             delay_between_polls_in_microseconds);

}


static INLINE void expand_ibuf_reg_LO_CYCLE_CFG_array_write( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_LO_CYCLE_CFG_array_write( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "expand_ibuf_reg_LO_CYCLE_CFG_array_write", value );
    expand_ibuf_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG(N),
                           value);
}

static INLINE void expand_ibuf_reg_LO_CYCLE_CFG_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_LO_CYCLE_CFG_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "expand_ibuf_reg_LO_CYCLE_CFG_array_field_set", N, mask, ofs, value );
    expand_ibuf_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG(N),
                           mask,
                           PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 expand_ibuf_reg_LO_CYCLE_CFG_array_read( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_reg_LO_CYCLE_CFG_array_read( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = expand_ibuf_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "expand_ibuf_reg_LO_CYCLE_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void expand_ibuf_reg_CHANNEL_CFG_REG_array_write( expand_ibuf_buffer_t *b_ptr,
                                                                expand_ibuf_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_CHANNEL_CFG_REG_array_write( expand_ibuf_buffer_t *b_ptr,
                                                                expand_ibuf_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "expand_ibuf_reg_CHANNEL_CFG_REG_array_write", value );
    expand_ibuf_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG(N),
                           value);
}

static INLINE void expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                    expand_ibuf_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                    expand_ibuf_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set", N, mask, ofs, value );
    expand_ibuf_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG(N),
                           mask,
                           PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 expand_ibuf_reg_CHANNEL_CFG_REG_array_read( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_reg_CHANNEL_CFG_REG_array_read( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = expand_ibuf_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "expand_ibuf_reg_CHANNEL_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void expand_ibuf_reg_TXJC_MGEN_CFGS_array_write( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_TXJC_MGEN_CFGS_array_write( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "expand_ibuf_reg_TXJC_MGEN_CFGS_array_write", value );
    expand_ibuf_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS(N),
                           value);
}

static INLINE void expand_ibuf_reg_TXJC_MGEN_CFGS_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                   expand_ibuf_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_TXJC_MGEN_CFGS_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                   expand_ibuf_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "expand_ibuf_reg_TXJC_MGEN_CFGS_array_field_set", N, mask, ofs, value );
    expand_ibuf_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS(N),
                           mask,
                           PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 expand_ibuf_reg_TXJC_MGEN_CFGS_array_read( expand_ibuf_buffer_t *b_ptr,
                                                                expand_ibuf_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_reg_TXJC_MGEN_CFGS_array_read( expand_ibuf_buffer_t *b_ptr,
                                                                expand_ibuf_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = expand_ibuf_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "expand_ibuf_reg_TXJC_MGEN_CFGS_array_read", reg_value, N);
    return reg_value;
}

static INLINE void expand_ibuf_reg_HAO_CFG_REG_array_write( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_HAO_CFG_REG_array_write( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "expand_ibuf_reg_HAO_CFG_REG_array_write", value );
    expand_ibuf_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_EXPAND_IBUF_REG_HAO_CFG_REG(N),
                           value);
}

static INLINE void expand_ibuf_reg_HAO_CFG_REG_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                expand_ibuf_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_HAO_CFG_REG_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                expand_ibuf_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "expand_ibuf_reg_HAO_CFG_REG_array_field_set", N, mask, ofs, value );
    expand_ibuf_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_EXPAND_IBUF_REG_HAO_CFG_REG(N),
                           mask,
                           PMC_EXPAND_IBUF_REG_HAO_CFG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 expand_ibuf_reg_HAO_CFG_REG_array_read( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_reg_HAO_CFG_REG_array_read( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = expand_ibuf_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_EXPAND_IBUF_REG_HAO_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "expand_ibuf_reg_HAO_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void expand_ibuf_reg_CFC_XOFF_LVL_array_write( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_CFC_XOFF_LVL_array_write( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "expand_ibuf_reg_CFC_XOFF_LVL_array_write", value );
    expand_ibuf_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL(N),
                           value);
}

static INLINE void expand_ibuf_reg_CFC_XOFF_LVL_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_CFC_XOFF_LVL_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "expand_ibuf_reg_CFC_XOFF_LVL_array_field_set", N, mask, ofs, value );
    expand_ibuf_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL(N),
                           mask,
                           PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 expand_ibuf_reg_CFC_XOFF_LVL_array_read( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_reg_CFC_XOFF_LVL_array_read( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = expand_ibuf_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "expand_ibuf_reg_CFC_XOFF_LVL_array_read", reg_value, N);
    return reg_value;
}

static INLINE void expand_ibuf_reg_SOMF_CFG_REG_array_write( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_SOMF_CFG_REG_array_write( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "expand_ibuf_reg_SOMF_CFG_REG_array_write", value );
    expand_ibuf_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_EXPAND_IBUF_REG_SOMF_CFG_REG(N),
                           value);
}

static INLINE void expand_ibuf_reg_SOMF_CFG_REG_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_reg_SOMF_CFG_REG_array_field_set( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "expand_ibuf_reg_SOMF_CFG_REG_array_field_set", N, mask, ofs, value );
    expand_ibuf_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_EXPAND_IBUF_REG_SOMF_CFG_REG(N),
                           mask,
                           PMC_EXPAND_IBUF_REG_SOMF_CFG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 expand_ibuf_reg_SOMF_CFG_REG_array_read( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_reg_SOMF_CFG_REG_array_read( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = expand_ibuf_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_EXPAND_IBUF_REG_SOMF_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "expand_ibuf_reg_SOMF_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void expand_ibuf_field_MEM_LO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                    expand_ibuf_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_MEM_LO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                    expand_ibuf_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_MEM_LO_ID_set", value, 127);
    IOLOG( "%s <= 0x%08x", "expand_ibuf_field_MEM_LO_ID_set", value );

    /* (0x00000360 bits 14:8) field MEM_LO_ID of register PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID */
    expand_ibuf_reg_MEM_CFG_STA_ID_field_set( b_ptr,
                                              h_ptr,
                                              EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_MSK,
                                              EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_OFF,
                                              value);
}

static INLINE UINT32 expand_ibuf_field_MEM_LO_ID_get( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_MEM_LO_ID_get( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000360 bits 14:8) field MEM_LO_ID of register PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID */
    reg_value = expand_ibuf_reg_MEM_CFG_STA_ID_read(  b_ptr, h_ptr);
    value = (reg_value & EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_MSK) >> EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_OFF;
    IOLOG( "%s -> 0x%08x", "expand_ibuf_field_MEM_LO_ID_get", value );

    return value;
}
static INLINE void expand_ibuf_field_range_MEM_LO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_MEM_LO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_MEM_LO_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_MEM_LO_ID_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_MEM_LO_ID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000360 bits 14:8) field MEM_LO_ID of register PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID */
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
        /* (0x00000360 bits 14:8) field MEM_LO_ID of register PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID */
        expand_ibuf_reg_MEM_CFG_STA_ID_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_OFF + subfield_offset),
                                                  EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_MEM_LO_ID_get( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_MEM_LO_ID_get( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_MEM_LO_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_MEM_LO_ID_get", stop_bit, 6 );
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
    /* (0x00000360 bits 14:8) field MEM_LO_ID of register PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID */
    reg_value = expand_ibuf_reg_MEM_CFG_STA_ID_read(  b_ptr, h_ptr);
    field_value = (reg_value & EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_MSK)
                  >> EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_MSK, EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_MEM_LO_ID_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_MEM_HO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                    expand_ibuf_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_MEM_HO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                    expand_ibuf_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_MEM_HO_ID_set", value, 127);
    IOLOG( "%s <= 0x%08x", "expand_ibuf_field_MEM_HO_ID_set", value );

    /* (0x00000360 bits 6:0) field MEM_HO_ID of register PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID */
    expand_ibuf_reg_MEM_CFG_STA_ID_field_set( b_ptr,
                                              h_ptr,
                                              EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_MSK,
                                              EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_OFF,
                                              value);
}

static INLINE UINT32 expand_ibuf_field_MEM_HO_ID_get( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_MEM_HO_ID_get( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000360 bits 6:0) field MEM_HO_ID of register PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID */
    reg_value = expand_ibuf_reg_MEM_CFG_STA_ID_read(  b_ptr, h_ptr);
    value = (reg_value & EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_MSK) >> EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_OFF;
    IOLOG( "%s -> 0x%08x", "expand_ibuf_field_MEM_HO_ID_get", value );

    return value;
}
static INLINE void expand_ibuf_field_range_MEM_HO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_MEM_HO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_MEM_HO_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_MEM_HO_ID_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_MEM_HO_ID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000360 bits 6:0) field MEM_HO_ID of register PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID */
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
        /* (0x00000360 bits 6:0) field MEM_HO_ID of register PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID */
        expand_ibuf_reg_MEM_CFG_STA_ID_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_OFF + subfield_offset),
                                                  EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_MEM_HO_ID_get( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_MEM_HO_ID_get( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_MEM_HO_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_MEM_HO_ID_get", stop_bit, 6 );
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
    /* (0x00000360 bits 6:0) field MEM_HO_ID of register PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID */
    reg_value = expand_ibuf_reg_MEM_CFG_STA_ID_read(  b_ptr, h_ptr);
    field_value = (reg_value & EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_MSK)
                  >> EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_MSK, EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_MEM_HO_ID_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void expand_ibuf_field_NORM_CMD_set( expand_ibuf_buffer_t *b_ptr,
                                                   expand_ibuf_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_NORM_CMD_set( expand_ibuf_buffer_t *b_ptr,
                                                   expand_ibuf_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_NORM_CMD_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_NORM_CMD_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_NORM_CMD_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 16) field NORM_CMD of register PMC_EXPAND_IBUF_REG_CFG_UPDATE index N=0..95 */
    expand_ibuf_reg_CFG_UPDATE_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                EXPAND_IBUF_REG_CFG_UPDATE_BIT_NORM_CMD_MSK,
                                                EXPAND_IBUF_REG_CFG_UPDATE_BIT_NORM_CMD_OFF,
                                                value);
}

static INLINE UINT32 expand_ibuf_field_NORM_CMD_get( expand_ibuf_buffer_t *b_ptr,
                                                     expand_ibuf_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_NORM_CMD_get( expand_ibuf_buffer_t *b_ptr,
                                                     expand_ibuf_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_NORM_CMD_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 16) field NORM_CMD of register PMC_EXPAND_IBUF_REG_CFG_UPDATE index N=0..95 */
    reg_value = expand_ibuf_reg_CFG_UPDATE_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & EXPAND_IBUF_REG_CFG_UPDATE_BIT_NORM_CMD_MSK) >> EXPAND_IBUF_REG_CFG_UPDATE_BIT_NORM_CMD_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_NORM_CMD_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_CONFIG_MODE_set( expand_ibuf_buffer_t *b_ptr,
                                                      expand_ibuf_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_CONFIG_MODE_set( expand_ibuf_buffer_t *b_ptr,
                                                      expand_ibuf_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_CONFIG_MODE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_CONFIG_MODE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_CONFIG_MODE_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 8) field CONFIG_MODE of register PMC_EXPAND_IBUF_REG_CFG_UPDATE index N=0..95 */
    expand_ibuf_reg_CFG_UPDATE_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_MODE_MSK,
                                                EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_MODE_OFF,
                                                value);
}

static INLINE UINT32 expand_ibuf_field_CONFIG_MODE_get( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_CONFIG_MODE_get( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_CONFIG_MODE_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 8) field CONFIG_MODE of register PMC_EXPAND_IBUF_REG_CFG_UPDATE index N=0..95 */
    reg_value = expand_ibuf_reg_CFG_UPDATE_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_MODE_MSK) >> EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_CONFIG_MODE_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_EOMF_DATA_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_EOMF_DATA_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_EOMF_DATA_CYCLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_EOMF_DATA_CYCLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_EOMF_DATA_CYCLE_set", N, value );

    /* ((0x00000800 + (N) * 0x4) bits 8) field EOMF_DATA_CYCLE of register PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG index N=0..95 */
    expand_ibuf_reg_LO_CYCLE_CFG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_EOMF_DATA_CYCLE_MSK,
                                                  EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_EOMF_DATA_CYCLE_OFF,
                                                  value);
}

static INLINE UINT32 expand_ibuf_field_EOMF_DATA_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_EOMF_DATA_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_EOMF_DATA_CYCLE_get", N, 95);
    /* ((0x00000800 + (N) * 0x4) bits 8) field EOMF_DATA_CYCLE of register PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG index N=0..95 */
    reg_value = expand_ibuf_reg_LO_CYCLE_CFG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_EOMF_DATA_CYCLE_MSK) >> EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_EOMF_DATA_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_EOMF_DATA_CYCLE_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_LO_FIRST_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_LO_FIRST_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_LO_FIRST_CYCLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_LO_FIRST_CYCLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_LO_FIRST_CYCLE_set", N, value );

    /* ((0x00000800 + (N) * 0x4) bits 0) field LO_FIRST_CYCLE of register PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG index N=0..95 */
    expand_ibuf_reg_LO_CYCLE_CFG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_LO_FIRST_CYCLE_MSK,
                                                  EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_LO_FIRST_CYCLE_OFF,
                                                  value);
}

static INLINE UINT32 expand_ibuf_field_LO_FIRST_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_LO_FIRST_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_LO_FIRST_CYCLE_get", N, 95);
    /* ((0x00000800 + (N) * 0x4) bits 0) field LO_FIRST_CYCLE of register PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG index N=0..95 */
    reg_value = expand_ibuf_reg_LO_CYCLE_CFG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_LO_FIRST_CYCLE_MSK) >> EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_LO_FIRST_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_LO_FIRST_CYCLE_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge_N_size96
 * ==================================================================================
 */
static INLINE void expand_ibuf_field_CONFIG_UPDATE_set( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_CONFIG_UPDATE_set( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_CONFIG_UPDATE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_CONFIG_UPDATE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_CONFIG_UPDATE_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 0) field CONFIG_UPDATE of register PMC_EXPAND_IBUF_REG_CFG_UPDATE index N=0..95 */
    expand_ibuf_reg_CFG_UPDATE_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_UPDATE_MSK,
                                                EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_UPDATE_OFF,
                                                value);
}

static INLINE UINT32 expand_ibuf_field_CONFIG_UPDATE_get( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_CONFIG_UPDATE_get( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_CONFIG_UPDATE_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 0) field CONFIG_UPDATE of register PMC_EXPAND_IBUF_REG_CFG_UPDATE index N=0..95 */
    reg_value = expand_ibuf_reg_CFG_UPDATE_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_UPDATE_MSK) >> EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_UPDATE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_CONFIG_UPDATE_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE expand_ibuf_field_CONFIG_UPDATE_poll( expand_ibuf_buffer_t *b_ptr,
                                                                         expand_ibuf_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE expand_ibuf_field_CONFIG_UPDATE_poll( expand_ibuf_buffer_t *b_ptr,
                                                                         expand_ibuf_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "expand_ibuf_field_CONFIG_UPDATE_poll", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 0) field CONFIG_UPDATE of register PMC_EXPAND_IBUF_REG_CFG_UPDATE index N=0..95 */
    return expand_ibuf_reg_CFG_UPDATE_array_poll( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_UPDATE_MSK,
                                                  (value<<EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_UPDATE_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void expand_ibuf_field_HO_CAL_CYCLE_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_HO_CAL_CYCLE_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_HO_CAL_CYCLE_TOT_set", N, 95);
    if (value > 32767)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_HO_CAL_CYCLE_TOT_set", value, 32767);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_HO_CAL_CYCLE_TOT_set", N, value );

    /* ((0x00000404 + (N) * 0x8) bits 31:17) field HO_CAL_CYCLE_TOT of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_MSK,
                                                     EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_OFF,
                                                     value);
}

static INLINE UINT32 expand_ibuf_field_HO_CAL_CYCLE_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_HO_CAL_CYCLE_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_HO_CAL_CYCLE_TOT_get", N, 95);
    /* ((0x00000404 + (N) * 0x8) bits 31:17) field HO_CAL_CYCLE_TOT of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_CHANNEL_CFG_REG_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_MSK) >> EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_HO_CAL_CYCLE_TOT_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_HO_CAL_CYCLE_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_HO_CAL_CYCLE_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_HO_CAL_CYCLE_TOT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_HO_CAL_CYCLE_TOT_set", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_HO_CAL_CYCLE_TOT_set", stop_bit, 14 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_HO_CAL_CYCLE_TOT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000404 + (N) * 0x8) bits 31:17) field HO_CAL_CYCLE_TOT of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
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
        if (stop_bit < 14) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 14;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000404 + (N) * 0x8) bits 31:17) field HO_CAL_CYCLE_TOT of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
        expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_OFF + subfield_offset),
                                                         EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_HO_CAL_CYCLE_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                                   expand_ibuf_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_HO_CAL_CYCLE_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                                   expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_HO_CAL_CYCLE_TOT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_HO_CAL_CYCLE_TOT_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_HO_CAL_CYCLE_TOT_get", stop_bit, 14 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 14) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 14;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000404 + (N) * 0x8) bits 31:17) field HO_CAL_CYCLE_TOT of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_CHANNEL_CFG_REG_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_MSK)
                  >> EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_MSK, EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_HO_CAL_CYCLE_TOT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_HO_LAST_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_HO_LAST_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_HO_LAST_CYCLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_HO_LAST_CYCLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_HO_LAST_CYCLE_set", N, value );

    /* ((0x00000404 + (N) * 0x8) bits 16) field HO_LAST_CYCLE of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_LAST_CYCLE_MSK,
                                                     EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_LAST_CYCLE_OFF,
                                                     value);
}

static INLINE UINT32 expand_ibuf_field_HO_LAST_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_HO_LAST_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_HO_LAST_CYCLE_get", N, 95);
    /* ((0x00000404 + (N) * 0x8) bits 16) field HO_LAST_CYCLE of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_CHANNEL_CFG_REG_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_LAST_CYCLE_MSK) >> EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_LAST_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_HO_LAST_CYCLE_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_HO_FIRST_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_HO_FIRST_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_HO_FIRST_CYCLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_HO_FIRST_CYCLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_HO_FIRST_CYCLE_set", N, value );

    /* ((0x00000404 + (N) * 0x8) bits 15) field HO_FIRST_CYCLE of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_FIRST_CYCLE_MSK,
                                                     EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_FIRST_CYCLE_OFF,
                                                     value);
}

static INLINE UINT32 expand_ibuf_field_HO_FIRST_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_HO_FIRST_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_HO_FIRST_CYCLE_get", N, 95);
    /* ((0x00000404 + (N) * 0x8) bits 15) field HO_FIRST_CYCLE of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_CHANNEL_CFG_REG_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_FIRST_CYCLE_MSK) >> EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_FIRST_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_HO_FIRST_CYCLE_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_HO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                expand_ibuf_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_HO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                expand_ibuf_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_HO_ID_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_HO_ID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_HO_ID_set", N, value );

    /* ((0x00000404 + (N) * 0x8) bits 14:8) field HO_ID of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_MSK,
                                                     EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_OFF,
                                                     value);
}

static INLINE UINT32 expand_ibuf_field_HO_ID_get( expand_ibuf_buffer_t *b_ptr,
                                                  expand_ibuf_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_HO_ID_get( expand_ibuf_buffer_t *b_ptr,
                                                  expand_ibuf_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_HO_ID_get", N, 95);
    /* ((0x00000404 + (N) * 0x8) bits 14:8) field HO_ID of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_CHANNEL_CFG_REG_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_MSK) >> EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_HO_ID_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_HO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                      expand_ibuf_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_HO_ID_set( expand_ibuf_buffer_t *b_ptr,
                                                      expand_ibuf_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_HO_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_HO_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_HO_ID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_HO_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000404 + (N) * 0x8) bits 14:8) field HO_ID of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
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
        /* ((0x00000404 + (N) * 0x8) bits 14:8) field HO_ID of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
        expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_OFF + subfield_offset),
                                                         EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_HO_ID_get( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_HO_ID_get( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_HO_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_HO_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_HO_ID_get", stop_bit, 6 );
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
    /* ((0x00000404 + (N) * 0x8) bits 14:8) field HO_ID of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_CHANNEL_CFG_REG_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_MSK)
                  >> EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_MSK, EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_HO_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_MF_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                 expand_ibuf_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_MF_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                 expand_ibuf_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_MF_TOT_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_MF_TOT_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_MF_TOT_set", N, value );

    /* ((0x00000404 + (N) * 0x8) bits 7:0) field MF_TOT of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_MSK,
                                                     EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_OFF,
                                                     value);
}

static INLINE UINT32 expand_ibuf_field_MF_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                   expand_ibuf_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_MF_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                   expand_ibuf_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_MF_TOT_get", N, 95);
    /* ((0x00000404 + (N) * 0x8) bits 7:0) field MF_TOT of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_CHANNEL_CFG_REG_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_MSK) >> EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_MF_TOT_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_MF_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                       expand_ibuf_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_MF_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                       expand_ibuf_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_MF_TOT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_MF_TOT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_MF_TOT_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_MF_TOT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000404 + (N) * 0x8) bits 7:0) field MF_TOT of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
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
        /* ((0x00000404 + (N) * 0x8) bits 7:0) field MF_TOT of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
        expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_OFF + subfield_offset),
                                                         EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_MF_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_MF_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_MF_TOT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_MF_TOT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_MF_TOT_get", stop_bit, 7 );
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
    /* ((0x00000404 + (N) * 0x8) bits 7:0) field MF_TOT of register PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_CHANNEL_CFG_REG_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_MSK)
                  >> EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_MSK, EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_MF_TOT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_CHAN_MAP_TYPE_set( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_CHAN_MAP_TYPE_set( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_CHAN_MAP_TYPE_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_CHAN_MAP_TYPE_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_CHAN_MAP_TYPE_set", N, value );

    /* ((0x00000c00 + (N) * 0x10) bits 30:28) field CHAN_MAP_TYPE of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
    expand_ibuf_reg_TXJC_MGEN_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_MSK,
                                                    EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_OFF,
                                                    value);
}

static INLINE UINT32 expand_ibuf_field_CHAN_MAP_TYPE_get( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_CHAN_MAP_TYPE_get( expand_ibuf_buffer_t *b_ptr,
                                                          expand_ibuf_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_CHAN_MAP_TYPE_get", N, 95);
    /* ((0x00000c00 + (N) * 0x10) bits 30:28) field CHAN_MAP_TYPE of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
    reg_value = expand_ibuf_reg_TXJC_MGEN_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_MSK) >> EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_CHAN_MAP_TYPE_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_CHAN_MAP_TYPE_set( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_CHAN_MAP_TYPE_set( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_CHAN_MAP_TYPE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_CHAN_MAP_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_CHAN_MAP_TYPE_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_CHAN_MAP_TYPE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c00 + (N) * 0x10) bits 30:28) field CHAN_MAP_TYPE of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
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
        /* ((0x00000c00 + (N) * 0x10) bits 30:28) field CHAN_MAP_TYPE of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
        expand_ibuf_reg_TXJC_MGEN_CFGS_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_OFF + subfield_offset),
                                                        EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_CHAN_MAP_TYPE_get( expand_ibuf_buffer_t *b_ptr,
                                                                expand_ibuf_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_CHAN_MAP_TYPE_get( expand_ibuf_buffer_t *b_ptr,
                                                                expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_CHAN_MAP_TYPE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_CHAN_MAP_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_CHAN_MAP_TYPE_get", stop_bit, 2 );
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
    /* ((0x00000c00 + (N) * 0x10) bits 30:28) field CHAN_MAP_TYPE of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
    reg_value = expand_ibuf_reg_TXJC_MGEN_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_MSK)
                  >> EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_MSK, EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_CHAN_MAP_TYPE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_M_set( expand_ibuf_buffer_t *b_ptr,
                                            expand_ibuf_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_M_set( expand_ibuf_buffer_t *b_ptr,
                                            expand_ibuf_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_M_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_M_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_M_set", N, value );

    /* ((0x00000c00 + (N) * 0x10) bits 27:21) field M of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
    expand_ibuf_reg_TXJC_MGEN_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_MSK,
                                                    EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_OFF,
                                                    value);
}

static INLINE UINT32 expand_ibuf_field_M_get( expand_ibuf_buffer_t *b_ptr,
                                              expand_ibuf_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_M_get( expand_ibuf_buffer_t *b_ptr,
                                              expand_ibuf_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_M_get", N, 95);
    /* ((0x00000c00 + (N) * 0x10) bits 27:21) field M of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
    reg_value = expand_ibuf_reg_TXJC_MGEN_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_MSK) >> EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_M_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_M_set( expand_ibuf_buffer_t *b_ptr,
                                                  expand_ibuf_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_M_set( expand_ibuf_buffer_t *b_ptr,
                                                  expand_ibuf_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_M_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_M_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_M_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_M_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c00 + (N) * 0x10) bits 27:21) field M of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
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
        /* ((0x00000c00 + (N) * 0x10) bits 27:21) field M of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
        expand_ibuf_reg_TXJC_MGEN_CFGS_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_OFF + subfield_offset),
                                                        EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_M_get( expand_ibuf_buffer_t *b_ptr,
                                                    expand_ibuf_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_M_get( expand_ibuf_buffer_t *b_ptr,
                                                    expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_M_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_M_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_M_get", stop_bit, 6 );
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
    /* ((0x00000c00 + (N) * 0x10) bits 27:21) field M of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
    reg_value = expand_ibuf_reg_TXJC_MGEN_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_MSK)
                  >> EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_MSK, EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_M_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_SERVER_SIZE_set( expand_ibuf_buffer_t *b_ptr,
                                                      expand_ibuf_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_SERVER_SIZE_set( expand_ibuf_buffer_t *b_ptr,
                                                      expand_ibuf_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_SERVER_SIZE_set", N, 95);
    if (value > 2097151)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_SERVER_SIZE_set", value, 2097151);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_SERVER_SIZE_set", N, value );

    /* ((0x00000c00 + (N) * 0x10) bits 20:0) field SERVER_SIZE of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
    expand_ibuf_reg_TXJC_MGEN_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_MSK,
                                                    EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_OFF,
                                                    value);
}

static INLINE UINT32 expand_ibuf_field_SERVER_SIZE_get( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_SERVER_SIZE_get( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_SERVER_SIZE_get", N, 95);
    /* ((0x00000c00 + (N) * 0x10) bits 20:0) field SERVER_SIZE of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
    reg_value = expand_ibuf_reg_TXJC_MGEN_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_MSK) >> EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_SERVER_SIZE_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_SERVER_SIZE_set( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_SERVER_SIZE_set( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_SERVER_SIZE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_SERVER_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 20) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_SERVER_SIZE_set", stop_bit, 20 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_SERVER_SIZE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c00 + (N) * 0x10) bits 20:0) field SERVER_SIZE of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
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
        if (stop_bit < 20) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 20;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000c00 + (N) * 0x10) bits 20:0) field SERVER_SIZE of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
        expand_ibuf_reg_TXJC_MGEN_CFGS_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_OFF + subfield_offset),
                                                        EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_SERVER_SIZE_get( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_SERVER_SIZE_get( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_SERVER_SIZE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_SERVER_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 20) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_SERVER_SIZE_get", stop_bit, 20 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 20) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 20;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000c00 + (N) * 0x10) bits 20:0) field SERVER_SIZE of register PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS index N=0..95 */
    reg_value = expand_ibuf_reg_TXJC_MGEN_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_MSK)
                  >> EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_MSK, EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_SERVER_SIZE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_ODTU_CNT_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                       expand_ibuf_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_ODTU_CNT_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                       expand_ibuf_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_ODTU_CNT_TOT_set", N, 95);
    if (value > 32767)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_ODTU_CNT_TOT_set", value, 32767);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_ODTU_CNT_TOT_set", N, value );

    /* ((0x00000c04 + (N) * 0x10) bits 31:17) field ODTU_CNT_TOT of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    expand_ibuf_reg_HAO_CFG_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_MSK,
                                                 EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_OFF,
                                                 value);
}

static INLINE UINT32 expand_ibuf_field_ODTU_CNT_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_ODTU_CNT_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_ODTU_CNT_TOT_get", N, 95);
    /* ((0x00000c04 + (N) * 0x10) bits 31:17) field ODTU_CNT_TOT of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_HAO_CFG_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_MSK) >> EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_ODTU_CNT_TOT_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_ODTU_CNT_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_ODTU_CNT_TOT_set( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_ODTU_CNT_TOT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_ODTU_CNT_TOT_set", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_ODTU_CNT_TOT_set", stop_bit, 14 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_ODTU_CNT_TOT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c04 + (N) * 0x10) bits 31:17) field ODTU_CNT_TOT of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
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
        if (stop_bit < 14) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 14;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000c04 + (N) * 0x10) bits 31:17) field ODTU_CNT_TOT of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
        expand_ibuf_reg_HAO_CFG_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_OFF + subfield_offset),
                                                     EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_ODTU_CNT_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_ODTU_CNT_TOT_get( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_ODTU_CNT_TOT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_ODTU_CNT_TOT_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_ODTU_CNT_TOT_get", stop_bit, 14 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 14) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 14;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000c04 + (N) * 0x10) bits 31:17) field ODTU_CNT_TOT of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_HAO_CFG_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_MSK)
                  >> EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_MSK, EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_ODTU_CNT_TOT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_EOMF_LASTCYCLE_DATA_set( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_EOMF_LASTCYCLE_DATA_set( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_EOMF_LASTCYCLE_DATA_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_EOMF_LASTCYCLE_DATA_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_EOMF_LASTCYCLE_DATA_set", N, value );

    /* ((0x00000c04 + (N) * 0x10) bits 16:10) field EOMF_LASTCYCLE_DATA of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    expand_ibuf_reg_HAO_CFG_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_MSK,
                                                 EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_OFF,
                                                 value);
}

static INLINE UINT32 expand_ibuf_field_EOMF_LASTCYCLE_DATA_get( expand_ibuf_buffer_t *b_ptr,
                                                                expand_ibuf_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_EOMF_LASTCYCLE_DATA_get( expand_ibuf_buffer_t *b_ptr,
                                                                expand_ibuf_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_EOMF_LASTCYCLE_DATA_get", N, 95);
    /* ((0x00000c04 + (N) * 0x10) bits 16:10) field EOMF_LASTCYCLE_DATA of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_HAO_CFG_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_MSK) >> EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_EOMF_LASTCYCLE_DATA_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_set( expand_ibuf_buffer_t *b_ptr,
                                                                    expand_ibuf_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_set( expand_ibuf_buffer_t *b_ptr,
                                                                    expand_ibuf_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c04 + (N) * 0x10) bits 16:10) field EOMF_LASTCYCLE_DATA of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
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
        /* ((0x00000c04 + (N) * 0x10) bits 16:10) field EOMF_LASTCYCLE_DATA of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
        expand_ibuf_reg_HAO_CFG_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_OFF + subfield_offset),
                                                     EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_get( expand_ibuf_buffer_t *b_ptr,
                                                                      expand_ibuf_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_get( expand_ibuf_buffer_t *b_ptr,
                                                                      expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_get", stop_bit, 6 );
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
    /* ((0x00000c04 + (N) * 0x10) bits 16:10) field EOMF_LASTCYCLE_DATA of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_HAO_CFG_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_MSK)
                  >> EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_MSK, EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_NUM_TRIG_set( expand_ibuf_buffer_t *b_ptr,
                                                   expand_ibuf_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_NUM_TRIG_set( expand_ibuf_buffer_t *b_ptr,
                                                   expand_ibuf_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_NUM_TRIG_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_NUM_TRIG_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_NUM_TRIG_set", N, value );

    /* ((0x00000c04 + (N) * 0x10) bits 9:7) field NUM_TRIG of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    expand_ibuf_reg_HAO_CFG_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_MSK,
                                                 EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_OFF,
                                                 value);
}

static INLINE UINT32 expand_ibuf_field_NUM_TRIG_get( expand_ibuf_buffer_t *b_ptr,
                                                     expand_ibuf_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_NUM_TRIG_get( expand_ibuf_buffer_t *b_ptr,
                                                     expand_ibuf_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_NUM_TRIG_get", N, 95);
    /* ((0x00000c04 + (N) * 0x10) bits 9:7) field NUM_TRIG of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_HAO_CFG_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_MSK) >> EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_NUM_TRIG_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_NUM_TRIG_set( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_NUM_TRIG_set( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_NUM_TRIG_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_NUM_TRIG_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_NUM_TRIG_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_NUM_TRIG_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c04 + (N) * 0x10) bits 9:7) field NUM_TRIG of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
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
        /* ((0x00000c04 + (N) * 0x10) bits 9:7) field NUM_TRIG of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
        expand_ibuf_reg_HAO_CFG_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_OFF + subfield_offset),
                                                     EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_NUM_TRIG_get( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_NUM_TRIG_get( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_NUM_TRIG_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_NUM_TRIG_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_NUM_TRIG_get", stop_bit, 2 );
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
    /* ((0x00000c04 + (N) * 0x10) bits 9:7) field NUM_TRIG of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_HAO_CFG_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_MSK)
                  >> EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_MSK, EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_NUM_TRIG_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_FIRST_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                      expand_ibuf_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_FIRST_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                      expand_ibuf_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_FIRST_CYCLE_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_FIRST_CYCLE_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_FIRST_CYCLE_set", N, value );

    /* ((0x00000c04 + (N) * 0x10) bits 6:0) field FIRST_CYCLE of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    expand_ibuf_reg_HAO_CFG_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_MSK,
                                                 EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_OFF,
                                                 value);
}

static INLINE UINT32 expand_ibuf_field_FIRST_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_FIRST_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                        expand_ibuf_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_FIRST_CYCLE_get", N, 95);
    /* ((0x00000c04 + (N) * 0x10) bits 6:0) field FIRST_CYCLE of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_HAO_CFG_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_MSK) >> EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_FIRST_CYCLE_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_FIRST_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_FIRST_CYCLE_set( expand_ibuf_buffer_t *b_ptr,
                                                            expand_ibuf_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_FIRST_CYCLE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_FIRST_CYCLE_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_FIRST_CYCLE_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_FIRST_CYCLE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c04 + (N) * 0x10) bits 6:0) field FIRST_CYCLE of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
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
        /* ((0x00000c04 + (N) * 0x10) bits 6:0) field FIRST_CYCLE of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
        expand_ibuf_reg_HAO_CFG_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_OFF + subfield_offset),
                                                     EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_FIRST_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_FIRST_CYCLE_get( expand_ibuf_buffer_t *b_ptr,
                                                              expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_FIRST_CYCLE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_FIRST_CYCLE_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_FIRST_CYCLE_get", stop_bit, 6 );
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
    /* ((0x00000c04 + (N) * 0x10) bits 6:0) field FIRST_CYCLE of register PMC_EXPAND_IBUF_REG_HAO_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_HAO_CFG_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_MSK)
                  >> EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_MSK, EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_FIRST_CYCLE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_CFC_FILL_LVL_set( expand_ibuf_buffer_t *b_ptr,
                                                       expand_ibuf_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_CFC_FILL_LVL_set( expand_ibuf_buffer_t *b_ptr,
                                                       expand_ibuf_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_CFC_FILL_LVL_set", N, 95);
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_CFC_FILL_LVL_set", value, 1023);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_CFC_FILL_LVL_set", N, value );

    /* ((0x00000c08 + (N) * 0x10) bits 9:0) field CFC_FILL_LVL of register PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL index N=0..95 */
    expand_ibuf_reg_CFC_XOFF_LVL_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_MSK,
                                                  EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_OFF,
                                                  value);
}

static INLINE UINT32 expand_ibuf_field_CFC_FILL_LVL_get( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_CFC_FILL_LVL_get( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_CFC_FILL_LVL_get", N, 95);
    /* ((0x00000c08 + (N) * 0x10) bits 9:0) field CFC_FILL_LVL of register PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL index N=0..95 */
    reg_value = expand_ibuf_reg_CFC_XOFF_LVL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_MSK) >> EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_CFC_FILL_LVL_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_CFC_FILL_LVL_set( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_CFC_FILL_LVL_set( expand_ibuf_buffer_t *b_ptr,
                                                             expand_ibuf_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_CFC_FILL_LVL_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_CFC_FILL_LVL_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_CFC_FILL_LVL_set", stop_bit, 9 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_CFC_FILL_LVL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c08 + (N) * 0x10) bits 9:0) field CFC_FILL_LVL of register PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL index N=0..95 */
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
        /* ((0x00000c08 + (N) * 0x10) bits 9:0) field CFC_FILL_LVL of register PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL index N=0..95 */
        expand_ibuf_reg_CFC_XOFF_LVL_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_OFF + subfield_offset),
                                                      EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_CFC_FILL_LVL_get( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_CFC_FILL_LVL_get( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_CFC_FILL_LVL_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_CFC_FILL_LVL_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_CFC_FILL_LVL_get", stop_bit, 9 );
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
    /* ((0x00000c08 + (N) * 0x10) bits 9:0) field CFC_FILL_LVL of register PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL index N=0..95 */
    reg_value = expand_ibuf_reg_CFC_XOFF_LVL_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_MSK)
                  >> EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_MSK, EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_CFC_FILL_LVL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_SOMF_FIRSTCYCLE_DATA_set( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_SOMF_FIRSTCYCLE_DATA_set( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_SOMF_FIRSTCYCLE_DATA_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_SOMF_FIRSTCYCLE_DATA_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_SOMF_FIRSTCYCLE_DATA_set", N, value );

    /* ((0x00001800 + (N) * 0x4) bits 22:16) field SOMF_FIRSTCYCLE_DATA of register PMC_EXPAND_IBUF_REG_SOMF_CFG_REG index N=0..95 */
    expand_ibuf_reg_SOMF_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_MSK,
                                                  EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_OFF,
                                                  value);
}

static INLINE UINT32 expand_ibuf_field_SOMF_FIRSTCYCLE_DATA_get( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_SOMF_FIRSTCYCLE_DATA_get( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_SOMF_FIRSTCYCLE_DATA_get", N, 95);
    /* ((0x00001800 + (N) * 0x4) bits 22:16) field SOMF_FIRSTCYCLE_DATA of register PMC_EXPAND_IBUF_REG_SOMF_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_SOMF_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_MSK) >> EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_SOMF_FIRSTCYCLE_DATA_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_set( expand_ibuf_buffer_t *b_ptr,
                                                                     expand_ibuf_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_set( expand_ibuf_buffer_t *b_ptr,
                                                                     expand_ibuf_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001800 + (N) * 0x4) bits 22:16) field SOMF_FIRSTCYCLE_DATA of register PMC_EXPAND_IBUF_REG_SOMF_CFG_REG index N=0..95 */
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
        /* ((0x00001800 + (N) * 0x4) bits 22:16) field SOMF_FIRSTCYCLE_DATA of register PMC_EXPAND_IBUF_REG_SOMF_CFG_REG index N=0..95 */
        expand_ibuf_reg_SOMF_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_OFF + subfield_offset),
                                                      EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_get( expand_ibuf_buffer_t *b_ptr,
                                                                       expand_ibuf_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_get( expand_ibuf_buffer_t *b_ptr,
                                                                       expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_get", stop_bit, 6 );
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
    /* ((0x00001800 + (N) * 0x4) bits 22:16) field SOMF_FIRSTCYCLE_DATA of register PMC_EXPAND_IBUF_REG_SOMF_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_SOMF_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_MSK)
                  >> EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_MSK, EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void expand_ibuf_field_SOMF_CYCLE_CNT_set( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_SOMF_CYCLE_CNT_set( expand_ibuf_buffer_t *b_ptr,
                                                         expand_ibuf_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_SOMF_CYCLE_CNT_set", N, 95);
    if (value > 32767)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "expand_ibuf_field_SOMF_CYCLE_CNT_set", value, 32767);
    IOLOG( "%s <= N=%d 0x%08x", "expand_ibuf_field_SOMF_CYCLE_CNT_set", N, value );

    /* ((0x00001800 + (N) * 0x4) bits 14:0) field SOMF_CYCLE_CNT of register PMC_EXPAND_IBUF_REG_SOMF_CFG_REG index N=0..95 */
    expand_ibuf_reg_SOMF_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_MSK,
                                                  EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_OFF,
                                                  value);
}

static INLINE UINT32 expand_ibuf_field_SOMF_CYCLE_CNT_get( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_SOMF_CYCLE_CNT_get( expand_ibuf_buffer_t *b_ptr,
                                                           expand_ibuf_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_SOMF_CYCLE_CNT_get", N, 95);
    /* ((0x00001800 + (N) * 0x4) bits 14:0) field SOMF_CYCLE_CNT of register PMC_EXPAND_IBUF_REG_SOMF_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_SOMF_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_MSK) >> EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "expand_ibuf_field_SOMF_CYCLE_CNT_get", N, value );

    return value;
}
static INLINE void expand_ibuf_field_range_SOMF_CYCLE_CNT_set( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void expand_ibuf_field_range_SOMF_CYCLE_CNT_set( expand_ibuf_buffer_t *b_ptr,
                                                               expand_ibuf_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_SOMF_CYCLE_CNT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_SOMF_CYCLE_CNT_set", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_SOMF_CYCLE_CNT_set", stop_bit, 14 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_SOMF_CYCLE_CNT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001800 + (N) * 0x4) bits 14:0) field SOMF_CYCLE_CNT of register PMC_EXPAND_IBUF_REG_SOMF_CFG_REG index N=0..95 */
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
        if (stop_bit < 14) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 14;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00001800 + (N) * 0x4) bits 14:0) field SOMF_CYCLE_CNT of register PMC_EXPAND_IBUF_REG_SOMF_CFG_REG index N=0..95 */
        expand_ibuf_reg_SOMF_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_OFF + subfield_offset),
                                                      EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 expand_ibuf_field_range_SOMF_CYCLE_CNT_get( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 expand_ibuf_field_range_SOMF_CYCLE_CNT_get( expand_ibuf_buffer_t *b_ptr,
                                                                 expand_ibuf_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "expand_ibuf_field_range_SOMF_CYCLE_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "expand_ibuf_field_range_SOMF_CYCLE_CNT_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "expand_ibuf_field_range_SOMF_CYCLE_CNT_get", stop_bit, 14 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 14) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 14;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00001800 + (N) * 0x4) bits 14:0) field SOMF_CYCLE_CNT of register PMC_EXPAND_IBUF_REG_SOMF_CFG_REG index N=0..95 */
    reg_value = expand_ibuf_reg_SOMF_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_MSK)
                  >> EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_MSK, EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "expand_ibuf_field_range_SOMF_CYCLE_CNT_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EXPAND_IBUF_IO_INLINE_H */
