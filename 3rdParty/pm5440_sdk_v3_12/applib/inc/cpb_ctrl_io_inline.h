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
 *     and register accessor functions for the cpb_ctrl block
 *****************************************************************************/
#ifndef _CPB_CTRL_IO_INLINE_H
#define _CPB_CTRL_IO_INLINE_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "cpb_ctrl_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CPB_CTRL_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for cpb_ctrl
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
    cpb_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} cpb_ctrl_buffer_t;
static INLINE void cpb_ctrl_buffer_init( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_buffer_init( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "cpb_ctrl_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void cpb_ctrl_buffer_flush( cpb_ctrl_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_buffer_flush( cpb_ctrl_buffer_t *b_ptr )
{
    IOLOG( "cpb_ctrl_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 cpb_ctrl_reg_read( cpb_ctrl_buffer_t *b_ptr,
                                        cpb_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_reg_read( cpb_ctrl_buffer_t *b_ptr,
                                        cpb_handle_t *h_ptr,
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
static INLINE void cpb_ctrl_reg_write( cpb_ctrl_buffer_t *b_ptr,
                                       cpb_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_write( cpb_ctrl_buffer_t *b_ptr,
                                       cpb_handle_t *h_ptr,
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

static INLINE void cpb_ctrl_field_set( cpb_ctrl_buffer_t *b_ptr,
                                       cpb_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_field_set( cpb_ctrl_buffer_t *b_ptr,
                                       cpb_handle_t *h_ptr,
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

static INLINE void cpb_ctrl_action_on_write_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_action_on_write_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
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

static INLINE void cpb_ctrl_burst_read( cpb_ctrl_buffer_t *b_ptr,
                                        cpb_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_burst_read( cpb_ctrl_buffer_t *b_ptr,
                                        cpb_handle_t *h_ptr,
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

static INLINE void cpb_ctrl_burst_write( cpb_ctrl_buffer_t *b_ptr,
                                         cpb_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_burst_write( cpb_ctrl_buffer_t *b_ptr,
                                         cpb_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE cpb_ctrl_poll( cpb_ctrl_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cpb_ctrl_poll( cpb_ctrl_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
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
 *  register access functions for cpb_ctrl
 * ==================================================================================
 */

static INLINE void cpb_ctrl_reg_MCPB_IMUX_write( cpb_ctrl_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MCPB_IMUX_write( cpb_ctrl_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpb_ctrl_reg_MCPB_IMUX_write", value );
    cpb_ctrl_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CPB_CTRL_REG_MCPB_IMUX,
                        value);
}

static INLINE void cpb_ctrl_reg_MCPB_IMUX_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MCPB_IMUX_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpb_ctrl_reg_MCPB_IMUX_field_set", mask, ofs, value );
    cpb_ctrl_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CPB_CTRL_REG_MCPB_IMUX,
                        mask,
                        PMC_CPB_CTRL_REG_MCPB_IMUX_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 cpb_ctrl_reg_MCPB_IMUX_read( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_reg_MCPB_IMUX_read( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpb_ctrl_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_CPB_CTRL_REG_MCPB_IMUX);

    IOLOG( "%s -> 0x%08x;", "cpb_ctrl_reg_MCPB_IMUX_read", reg_value);
    return reg_value;
}

static INLINE void cpb_ctrl_reg_MCPB_EMUX_write( cpb_ctrl_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MCPB_EMUX_write( cpb_ctrl_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpb_ctrl_reg_MCPB_EMUX_write", value );
    cpb_ctrl_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CPB_CTRL_REG_MCPB_EMUX,
                        value);
}

static INLINE void cpb_ctrl_reg_MCPB_EMUX_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MCPB_EMUX_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpb_ctrl_reg_MCPB_EMUX_field_set", mask, ofs, value );
    cpb_ctrl_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CPB_CTRL_REG_MCPB_EMUX,
                        mask,
                        PMC_CPB_CTRL_REG_MCPB_EMUX_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 cpb_ctrl_reg_MCPB_EMUX_read( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_reg_MCPB_EMUX_read( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpb_ctrl_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_CPB_CTRL_REG_MCPB_EMUX);

    IOLOG( "%s -> 0x%08x;", "cpb_ctrl_reg_MCPB_EMUX_read", reg_value);
    return reg_value;
}

static INLINE void cpb_ctrl_reg_DCPB_MUX_write( cpb_ctrl_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_DCPB_MUX_write( cpb_ctrl_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpb_ctrl_reg_DCPB_MUX_write", value );
    cpb_ctrl_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CPB_CTRL_REG_DCPB_MUX,
                        value);
}

static INLINE void cpb_ctrl_reg_DCPB_MUX_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_DCPB_MUX_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpb_ctrl_reg_DCPB_MUX_field_set", mask, ofs, value );
    cpb_ctrl_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CPB_CTRL_REG_DCPB_MUX,
                        mask,
                        PMC_CPB_CTRL_REG_DCPB_MUX_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 cpb_ctrl_reg_DCPB_MUX_read( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_reg_DCPB_MUX_read( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpb_ctrl_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_CPB_CTRL_REG_DCPB_MUX);

    IOLOG( "%s -> 0x%08x;", "cpb_ctrl_reg_DCPB_MUX_read", reg_value);
    return reg_value;
}

static INLINE void cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_write( cpb_ctrl_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_write( cpb_ctrl_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 8)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_write", ofs, len, 8 );
    IOLOG( "%s of %d words ofs=%d len=%d", "cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_write", 8, ofs, len);
    cpb_ctrl_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_CPB_CTRL_REG_MASK_DCS_0_REG(ofs),
                          len,
                          value);

}

static INLINE void cpb_ctrl_reg_MASK_DCS_0_REG_array_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_0_REG_array_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpb_ctrl_reg_MASK_DCS_0_REG_array_field_set", mask, ofs, value );
    cpb_ctrl_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CPB_CTRL_REG_MASK_DCS_0_REG(N),
                        mask,
                        PMC_CPB_CTRL_REG_MASK_DCS_0_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_read( cpb_ctrl_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_read( cpb_ctrl_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value )
{
    if (len + ofs > 8)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_read", ofs, len, 8 );
    cpb_ctrl_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CPB_CTRL_REG_MASK_DCS_0_REG(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_read", 8, ofs, len);
}

static INLINE UINT32 cpb_ctrl_reg_MASK_DCS_0_REG_array_read( cpb_ctrl_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_reg_MASK_DCS_0_REG_array_read( cpb_ctrl_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = cpb_ctrl_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_CPB_CTRL_REG_MASK_DCS_0_REG(N));

    IOLOG( "%s -> 0x%08x;", "cpb_ctrl_reg_MASK_DCS_0_REG_array_read", reg_value);
    return reg_value;
}

static INLINE void cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_write( cpb_ctrl_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_write( cpb_ctrl_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 8)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_write", ofs, len, 8 );
    IOLOG( "%s of %d words ofs=%d len=%d", "cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_write", 8, ofs, len);
    cpb_ctrl_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_CPB_CTRL_REG_MASK_DCS_1_REG(ofs),
                          len,
                          value);

}

static INLINE void cpb_ctrl_reg_MASK_DCS_1_REG_array_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_1_REG_array_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpb_ctrl_reg_MASK_DCS_1_REG_array_field_set", mask, ofs, value );
    cpb_ctrl_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CPB_CTRL_REG_MASK_DCS_1_REG(N),
                        mask,
                        PMC_CPB_CTRL_REG_MASK_DCS_1_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_read( cpb_ctrl_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_read( cpb_ctrl_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value )
{
    if (len + ofs > 8)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_read", ofs, len, 8 );
    cpb_ctrl_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CPB_CTRL_REG_MASK_DCS_1_REG(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_read", 8, ofs, len);
}

static INLINE UINT32 cpb_ctrl_reg_MASK_DCS_1_REG_array_read( cpb_ctrl_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_reg_MASK_DCS_1_REG_array_read( cpb_ctrl_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = cpb_ctrl_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_CPB_CTRL_REG_MASK_DCS_1_REG(N));

    IOLOG( "%s -> 0x%08x;", "cpb_ctrl_reg_MASK_DCS_1_REG_array_read", reg_value);
    return reg_value;
}

static INLINE void cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_write( cpb_ctrl_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_write( cpb_ctrl_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 8)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_write", ofs, len, 8 );
    IOLOG( "%s of %d words ofs=%d len=%d", "cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_write", 8, ofs, len);
    cpb_ctrl_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_CPB_CTRL_REG_MASK_DCS_2_REG(ofs),
                          len,
                          value);

}

static INLINE void cpb_ctrl_reg_MASK_DCS_2_REG_array_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_2_REG_array_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpb_ctrl_reg_MASK_DCS_2_REG_array_field_set", mask, ofs, value );
    cpb_ctrl_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CPB_CTRL_REG_MASK_DCS_2_REG(N),
                        mask,
                        PMC_CPB_CTRL_REG_MASK_DCS_2_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_read( cpb_ctrl_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_read( cpb_ctrl_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value )
{
    if (len + ofs > 8)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_read", ofs, len, 8 );
    cpb_ctrl_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CPB_CTRL_REG_MASK_DCS_2_REG(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_read", 8, ofs, len);
}

static INLINE UINT32 cpb_ctrl_reg_MASK_DCS_2_REG_array_read( cpb_ctrl_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_reg_MASK_DCS_2_REG_array_read( cpb_ctrl_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = cpb_ctrl_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_CPB_CTRL_REG_MASK_DCS_2_REG(N));

    IOLOG( "%s -> 0x%08x;", "cpb_ctrl_reg_MASK_DCS_2_REG_array_read", reg_value);
    return reg_value;
}

static INLINE void cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_write( cpb_ctrl_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_write( cpb_ctrl_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 8)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_write", ofs, len, 8 );
    IOLOG( "%s of %d words ofs=%d len=%d", "cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_write", 8, ofs, len);
    cpb_ctrl_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_CPB_CTRL_REG_MASK_DCS_3_REG(ofs),
                          len,
                          value);

}

static INLINE void cpb_ctrl_reg_MASK_DCS_3_REG_array_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_3_REG_array_field_set( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpb_ctrl_reg_MASK_DCS_3_REG_array_field_set", mask, ofs, value );
    cpb_ctrl_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CPB_CTRL_REG_MASK_DCS_3_REG(N),
                        mask,
                        PMC_CPB_CTRL_REG_MASK_DCS_3_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_read( cpb_ctrl_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_read( cpb_ctrl_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value )
{
    if (len + ofs > 8)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_read", ofs, len, 8 );
    cpb_ctrl_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CPB_CTRL_REG_MASK_DCS_3_REG(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_read", 8, ofs, len);
}

static INLINE UINT32 cpb_ctrl_reg_MASK_DCS_3_REG_array_read( cpb_ctrl_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_reg_MASK_DCS_3_REG_array_read( cpb_ctrl_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = cpb_ctrl_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_CPB_CTRL_REG_MASK_DCS_3_REG(N));

    IOLOG( "%s -> 0x%08x;", "cpb_ctrl_reg_MASK_DCS_3_REG_array_read", reg_value);
    return reg_value;
}

static INLINE UINT32 cpb_ctrl_reg_INTSUM_read( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_reg_INTSUM_read( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpb_ctrl_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_CPB_CTRL_REG_INTSUM);

    IOLOG( "%s -> 0x%08x;", "cpb_ctrl_reg_INTSUM_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void cpb_ctrl_field_IM_IGR_EXT_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_field_IM_IGR_EXT_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpb_ctrl_field_IM_IGR_EXT_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpb_ctrl_field_IM_IGR_EXT_SEL_set", value );

    /* (0x00041000 bits 16) field IM_IGR_EXT_SEL of register PMC_CPB_CTRL_REG_MCPB_IMUX */
    cpb_ctrl_reg_MCPB_IMUX_field_set( b_ptr,
                                      h_ptr,
                                      CPB_CTRL_REG_MCPB_IMUX_BIT_IM_IGR_EXT_SEL_MSK,
                                      CPB_CTRL_REG_MCPB_IMUX_BIT_IM_IGR_EXT_SEL_OFF,
                                      value);
}

static INLINE UINT32 cpb_ctrl_field_IM_IGR_EXT_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_field_IM_IGR_EXT_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00041000 bits 16) field IM_IGR_EXT_SEL of register PMC_CPB_CTRL_REG_MCPB_IMUX */
    reg_value = cpb_ctrl_reg_MCPB_IMUX_read(  b_ptr, h_ptr);
    value = (reg_value & CPB_CTRL_REG_MCPB_IMUX_BIT_IM_IGR_EXT_SEL_MSK) >> CPB_CTRL_REG_MCPB_IMUX_BIT_IM_IGR_EXT_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "cpb_ctrl_field_IM_IGR_EXT_SEL_get", value );

    return value;
}
static INLINE void cpb_ctrl_field_IM_EGR_EXT_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_field_IM_EGR_EXT_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpb_ctrl_field_IM_EGR_EXT_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpb_ctrl_field_IM_EGR_EXT_SEL_set", value );

    /* (0x00041000 bits 8) field IM_EGR_EXT_SEL of register PMC_CPB_CTRL_REG_MCPB_IMUX */
    cpb_ctrl_reg_MCPB_IMUX_field_set( b_ptr,
                                      h_ptr,
                                      CPB_CTRL_REG_MCPB_IMUX_BIT_IM_EGR_EXT_SEL_MSK,
                                      CPB_CTRL_REG_MCPB_IMUX_BIT_IM_EGR_EXT_SEL_OFF,
                                      value);
}

static INLINE UINT32 cpb_ctrl_field_IM_EGR_EXT_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_field_IM_EGR_EXT_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00041000 bits 8) field IM_EGR_EXT_SEL of register PMC_CPB_CTRL_REG_MCPB_IMUX */
    reg_value = cpb_ctrl_reg_MCPB_IMUX_read(  b_ptr, h_ptr);
    value = (reg_value & CPB_CTRL_REG_MCPB_IMUX_BIT_IM_EGR_EXT_SEL_MSK) >> CPB_CTRL_REG_MCPB_IMUX_BIT_IM_EGR_EXT_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "cpb_ctrl_field_IM_EGR_EXT_SEL_get", value );

    return value;
}
static INLINE void cpb_ctrl_field_IM_REFLECT_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_field_IM_REFLECT_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpb_ctrl_field_IM_REFLECT_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpb_ctrl_field_IM_REFLECT_SEL_set", value );

    /* (0x00041000 bits 0) field IM_REFLECT_SEL of register PMC_CPB_CTRL_REG_MCPB_IMUX */
    cpb_ctrl_reg_MCPB_IMUX_field_set( b_ptr,
                                      h_ptr,
                                      CPB_CTRL_REG_MCPB_IMUX_BIT_IM_REFLECT_SEL_MSK,
                                      CPB_CTRL_REG_MCPB_IMUX_BIT_IM_REFLECT_SEL_OFF,
                                      value);
}

static INLINE UINT32 cpb_ctrl_field_IM_REFLECT_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_field_IM_REFLECT_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00041000 bits 0) field IM_REFLECT_SEL of register PMC_CPB_CTRL_REG_MCPB_IMUX */
    reg_value = cpb_ctrl_reg_MCPB_IMUX_read(  b_ptr, h_ptr);
    value = (reg_value & CPB_CTRL_REG_MCPB_IMUX_BIT_IM_REFLECT_SEL_MSK) >> CPB_CTRL_REG_MCPB_IMUX_BIT_IM_REFLECT_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "cpb_ctrl_field_IM_REFLECT_SEL_get", value );

    return value;
}
static INLINE void cpb_ctrl_field_EM_EGR_EXT_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_field_EM_EGR_EXT_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpb_ctrl_field_EM_EGR_EXT_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpb_ctrl_field_EM_EGR_EXT_SEL_set", value );

    /* (0x00041004 bits 16) field EM_EGR_EXT_SEL of register PMC_CPB_CTRL_REG_MCPB_EMUX */
    cpb_ctrl_reg_MCPB_EMUX_field_set( b_ptr,
                                      h_ptr,
                                      CPB_CTRL_REG_MCPB_EMUX_BIT_EM_EGR_EXT_SEL_MSK,
                                      CPB_CTRL_REG_MCPB_EMUX_BIT_EM_EGR_EXT_SEL_OFF,
                                      value);
}

static INLINE UINT32 cpb_ctrl_field_EM_EGR_EXT_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_field_EM_EGR_EXT_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00041004 bits 16) field EM_EGR_EXT_SEL of register PMC_CPB_CTRL_REG_MCPB_EMUX */
    reg_value = cpb_ctrl_reg_MCPB_EMUX_read(  b_ptr, h_ptr);
    value = (reg_value & CPB_CTRL_REG_MCPB_EMUX_BIT_EM_EGR_EXT_SEL_MSK) >> CPB_CTRL_REG_MCPB_EMUX_BIT_EM_EGR_EXT_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "cpb_ctrl_field_EM_EGR_EXT_SEL_get", value );

    return value;
}
static INLINE void cpb_ctrl_field_EM_IGR_EXT_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_field_EM_IGR_EXT_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpb_ctrl_field_EM_IGR_EXT_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpb_ctrl_field_EM_IGR_EXT_SEL_set", value );

    /* (0x00041004 bits 8) field EM_IGR_EXT_SEL of register PMC_CPB_CTRL_REG_MCPB_EMUX */
    cpb_ctrl_reg_MCPB_EMUX_field_set( b_ptr,
                                      h_ptr,
                                      CPB_CTRL_REG_MCPB_EMUX_BIT_EM_IGR_EXT_SEL_MSK,
                                      CPB_CTRL_REG_MCPB_EMUX_BIT_EM_IGR_EXT_SEL_OFF,
                                      value);
}

static INLINE UINT32 cpb_ctrl_field_EM_IGR_EXT_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_field_EM_IGR_EXT_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00041004 bits 8) field EM_IGR_EXT_SEL of register PMC_CPB_CTRL_REG_MCPB_EMUX */
    reg_value = cpb_ctrl_reg_MCPB_EMUX_read(  b_ptr, h_ptr);
    value = (reg_value & CPB_CTRL_REG_MCPB_EMUX_BIT_EM_IGR_EXT_SEL_MSK) >> CPB_CTRL_REG_MCPB_EMUX_BIT_EM_IGR_EXT_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "cpb_ctrl_field_EM_IGR_EXT_SEL_get", value );

    return value;
}
static INLINE void cpb_ctrl_field_IGR_SIFD2_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_field_IGR_SIFD2_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpb_ctrl_field_IGR_SIFD2_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpb_ctrl_field_IGR_SIFD2_SEL_set", value );

    /* (0x00041014 bits 9) field IGR_SIFD2_SEL of register PMC_CPB_CTRL_REG_DCPB_MUX */
    cpb_ctrl_reg_DCPB_MUX_field_set( b_ptr,
                                     h_ptr,
                                     CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD2_SEL_MSK,
                                     CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD2_SEL_OFF,
                                     value);
}

static INLINE UINT32 cpb_ctrl_field_IGR_SIFD2_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_field_IGR_SIFD2_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00041014 bits 9) field IGR_SIFD2_SEL of register PMC_CPB_CTRL_REG_DCPB_MUX */
    reg_value = cpb_ctrl_reg_DCPB_MUX_read(  b_ptr, h_ptr);
    value = (reg_value & CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD2_SEL_MSK) >> CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD2_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "cpb_ctrl_field_IGR_SIFD2_SEL_get", value );

    return value;
}
static INLINE void cpb_ctrl_field_EGR_SIFD2_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_field_EGR_SIFD2_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpb_ctrl_field_EGR_SIFD2_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpb_ctrl_field_EGR_SIFD2_SEL_set", value );

    /* (0x00041014 bits 8) field EGR_SIFD2_SEL of register PMC_CPB_CTRL_REG_DCPB_MUX */
    cpb_ctrl_reg_DCPB_MUX_field_set( b_ptr,
                                     h_ptr,
                                     CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD2_SEL_MSK,
                                     CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD2_SEL_OFF,
                                     value);
}

static INLINE UINT32 cpb_ctrl_field_EGR_SIFD2_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_field_EGR_SIFD2_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00041014 bits 8) field EGR_SIFD2_SEL of register PMC_CPB_CTRL_REG_DCPB_MUX */
    reg_value = cpb_ctrl_reg_DCPB_MUX_read(  b_ptr, h_ptr);
    value = (reg_value & CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD2_SEL_MSK) >> CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD2_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "cpb_ctrl_field_EGR_SIFD2_SEL_get", value );

    return value;
}
static INLINE void cpb_ctrl_field_IGR_SIFD1_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_field_IGR_SIFD1_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpb_ctrl_field_IGR_SIFD1_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpb_ctrl_field_IGR_SIFD1_SEL_set", value );

    /* (0x00041014 bits 1) field IGR_SIFD1_SEL of register PMC_CPB_CTRL_REG_DCPB_MUX */
    cpb_ctrl_reg_DCPB_MUX_field_set( b_ptr,
                                     h_ptr,
                                     CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD1_SEL_MSK,
                                     CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD1_SEL_OFF,
                                     value);
}

static INLINE UINT32 cpb_ctrl_field_IGR_SIFD1_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_field_IGR_SIFD1_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00041014 bits 1) field IGR_SIFD1_SEL of register PMC_CPB_CTRL_REG_DCPB_MUX */
    reg_value = cpb_ctrl_reg_DCPB_MUX_read(  b_ptr, h_ptr);
    value = (reg_value & CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD1_SEL_MSK) >> CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD1_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "cpb_ctrl_field_IGR_SIFD1_SEL_get", value );

    return value;
}
static INLINE void cpb_ctrl_field_EGR_SIFD1_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_field_EGR_SIFD1_SEL_set( cpb_ctrl_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpb_ctrl_field_EGR_SIFD1_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpb_ctrl_field_EGR_SIFD1_SEL_set", value );

    /* (0x00041014 bits 0) field EGR_SIFD1_SEL of register PMC_CPB_CTRL_REG_DCPB_MUX */
    cpb_ctrl_reg_DCPB_MUX_field_set( b_ptr,
                                     h_ptr,
                                     CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD1_SEL_MSK,
                                     CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD1_SEL_OFF,
                                     value);
}

static INLINE UINT32 cpb_ctrl_field_EGR_SIFD1_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_field_EGR_SIFD1_SEL_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00041014 bits 0) field EGR_SIFD1_SEL of register PMC_CPB_CTRL_REG_DCPB_MUX */
    reg_value = cpb_ctrl_reg_DCPB_MUX_read(  b_ptr, h_ptr);
    value = (reg_value & CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD1_SEL_MSK) >> CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD1_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "cpb_ctrl_field_EGR_SIFD1_SEL_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_burstsize8
 * ==================================================================================
 */
static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_0_set( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_0_set( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] )
{
    IOLOG( "%s ", "cpb_ctrl_lfield_XOFF_MASK_DCS_0_set");
    cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_write( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   8,
                                                   value);
}

static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_0_get( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_0_get( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] )
{
    IOLOG( "%s ", "cpb_ctrl_lfield_XOFF_MASK_DCS_0_get");
    cpb_ctrl_reg_MASK_DCS_0_REG_array_burst_read( b_ptr,
                                                  h_ptr,
                                                  0,
                                                  8,
                                                  (UINT32 *)value);
}

static INLINE void cpb_ctrl_lfield_range_XOFF_MASK_DCS_0_set( cpb_ctrl_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_range_XOFF_MASK_DCS_0_set( cpb_ctrl_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_0_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_0_set", start_bit, stop_bit, value);
    cpb_ctrl_reg_MASK_DCS_0_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 word_number,
                                                 mask,
                                                 field_ofs,
                                                 value);
}

static INLINE UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_0_get( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_0_get( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_0_get", start_bit, stop_bit );
    }
    reg_value = cpb_ctrl_reg_MASK_DCS_0_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_0_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_1_set( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_1_set( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] )
{
    IOLOG( "%s ", "cpb_ctrl_lfield_XOFF_MASK_DCS_1_set");
    cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_write( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   8,
                                                   value);
}

static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_1_get( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_1_get( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] )
{
    IOLOG( "%s ", "cpb_ctrl_lfield_XOFF_MASK_DCS_1_get");
    cpb_ctrl_reg_MASK_DCS_1_REG_array_burst_read( b_ptr,
                                                  h_ptr,
                                                  0,
                                                  8,
                                                  (UINT32 *)value);
}

static INLINE void cpb_ctrl_lfield_range_XOFF_MASK_DCS_1_set( cpb_ctrl_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_range_XOFF_MASK_DCS_1_set( cpb_ctrl_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_1_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_1_set", start_bit, stop_bit, value);
    cpb_ctrl_reg_MASK_DCS_1_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 word_number,
                                                 mask,
                                                 field_ofs,
                                                 value);
}

static INLINE UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_1_get( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_1_get( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_1_get", start_bit, stop_bit );
    }
    reg_value = cpb_ctrl_reg_MASK_DCS_1_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_1_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_2_set( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_2_set( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] )
{
    IOLOG( "%s ", "cpb_ctrl_lfield_XOFF_MASK_DCS_2_set");
    cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_write( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   8,
                                                   value);
}

static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_2_get( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_2_get( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] )
{
    IOLOG( "%s ", "cpb_ctrl_lfield_XOFF_MASK_DCS_2_get");
    cpb_ctrl_reg_MASK_DCS_2_REG_array_burst_read( b_ptr,
                                                  h_ptr,
                                                  0,
                                                  8,
                                                  (UINT32 *)value);
}

static INLINE void cpb_ctrl_lfield_range_XOFF_MASK_DCS_2_set( cpb_ctrl_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_range_XOFF_MASK_DCS_2_set( cpb_ctrl_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_2_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_2_set", start_bit, stop_bit, value);
    cpb_ctrl_reg_MASK_DCS_2_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 word_number,
                                                 mask,
                                                 field_ofs,
                                                 value);
}

static INLINE UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_2_get( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_2_get( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_2_get", start_bit, stop_bit );
    }
    reg_value = cpb_ctrl_reg_MASK_DCS_2_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_2_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_3_set( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_3_set( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] )
{
    IOLOG( "%s ", "cpb_ctrl_lfield_XOFF_MASK_DCS_3_set");
    cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_write( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   8,
                                                   value);
}

static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_3_get( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_XOFF_MASK_DCS_3_get( cpb_ctrl_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value[8] )
{
    IOLOG( "%s ", "cpb_ctrl_lfield_XOFF_MASK_DCS_3_get");
    cpb_ctrl_reg_MASK_DCS_3_REG_array_burst_read( b_ptr,
                                                  h_ptr,
                                                  0,
                                                  8,
                                                  (UINT32 *)value);
}

static INLINE void cpb_ctrl_lfield_range_XOFF_MASK_DCS_3_set( cpb_ctrl_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cpb_ctrl_lfield_range_XOFF_MASK_DCS_3_set( cpb_ctrl_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_3_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_3_set", start_bit, stop_bit, value);
    cpb_ctrl_reg_MASK_DCS_3_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 word_number,
                                                 mask,
                                                 field_ofs,
                                                 value);
}

static INLINE UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_3_get( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_lfield_range_XOFF_MASK_DCS_3_get( cpb_ctrl_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_3_get", start_bit, stop_bit );
    }
    reg_value = cpb_ctrl_reg_MASK_DCS_3_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "cpb_ctrl_lfield_range_XOFF_MASK_DCS_3_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 cpb_ctrl_field_SLV_DPI_INT_SUM_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_field_SLV_DPI_INT_SUM_get( cpb_ctrl_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00040110 bits 23:16) bits 0:7 use field SLV_DPI_INT_SUM of register PMC_CPB_CTRL_REG_INTSUM */
    reg_value = cpb_ctrl_reg_INTSUM_read(  b_ptr, h_ptr);
    value = (reg_value & CPB_CTRL_REG_INTSUM_BIT_SLV_DPI_INT_SUM_MSK) >> CPB_CTRL_REG_INTSUM_BIT_SLV_DPI_INT_SUM_OFF;
    IOLOG( "%s -> 0x%08x", "cpb_ctrl_field_SLV_DPI_INT_SUM_get", value );

    return value;
}
static INLINE UINT32 cpb_ctrl_field_range_SLV_DPI_INT_SUM_get( cpb_ctrl_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpb_ctrl_field_range_SLV_DPI_INT_SUM_get( cpb_ctrl_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpb_ctrl_field_range_SLV_DPI_INT_SUM_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpb_ctrl_field_range_SLV_DPI_INT_SUM_get", stop_bit, 7 );
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
    /* (0x00040110 bits 23:16) bits 0:7 use field SLV_DPI_INT_SUM of register PMC_CPB_CTRL_REG_INTSUM */
    reg_value = cpb_ctrl_reg_INTSUM_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPB_CTRL_REG_INTSUM_BIT_SLV_DPI_INT_SUM_MSK)
                  >> CPB_CTRL_REG_INTSUM_BIT_SLV_DPI_INT_SUM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPB_CTRL_REG_INTSUM_BIT_SLV_DPI_INT_SUM_MSK, CPB_CTRL_REG_INTSUM_BIT_SLV_DPI_INT_SUM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpb_ctrl_field_range_SLV_DPI_INT_SUM_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CPB_CTRL_IO_INLINE_H */
