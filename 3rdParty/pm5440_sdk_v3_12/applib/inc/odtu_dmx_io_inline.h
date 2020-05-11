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
 *     and register accessor functions for the odtu_dmx block
 *****************************************************************************/
#ifndef _ODTU_DMX_IO_INLINE_H
#define _ODTU_DMX_IO_INLINE_H

#include "odtu_dmx_loc.h"
#include "odtu_dmx_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ODTU_DMX_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for odtu_dmx
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
    odtu_dmx_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} odtu_dmx_buffer_t;
static INLINE void odtu_dmx_buffer_init( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void odtu_dmx_buffer_init( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "odtu_dmx_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void odtu_dmx_buffer_flush( odtu_dmx_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void odtu_dmx_buffer_flush( odtu_dmx_buffer_t *b_ptr )
{
    IOLOG( "odtu_dmx_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 odtu_dmx_reg_read( odtu_dmx_buffer_t *b_ptr,
                                        odtu_dmx_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_read( odtu_dmx_buffer_t *b_ptr,
                                        odtu_dmx_handle_t *h_ptr,
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
static INLINE void odtu_dmx_reg_write( odtu_dmx_buffer_t *b_ptr,
                                       odtu_dmx_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_write( odtu_dmx_buffer_t *b_ptr,
                                       odtu_dmx_handle_t *h_ptr,
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

static INLINE void odtu_dmx_field_set( odtu_dmx_buffer_t *b_ptr,
                                       odtu_dmx_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_set( odtu_dmx_buffer_t *b_ptr,
                                       odtu_dmx_handle_t *h_ptr,
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

static INLINE void odtu_dmx_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
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

static INLINE void odtu_dmx_burst_read( odtu_dmx_buffer_t *b_ptr,
                                        odtu_dmx_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_burst_read( odtu_dmx_buffer_t *b_ptr,
                                        odtu_dmx_handle_t *h_ptr,
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

static INLINE void odtu_dmx_burst_write( odtu_dmx_buffer_t *b_ptr,
                                         odtu_dmx_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_burst_write( odtu_dmx_buffer_t *b_ptr,
                                         odtu_dmx_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_poll( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_poll( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
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
 *  register access functions for odtu_dmx
 * ==================================================================================
 */

static INLINE void odtu_dmx_reg_FORCE_CONFIG_REG_write( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_FORCE_CONFIG_REG_write( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_FORCE_CONFIG_REG_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_FORCE_CONFIG_REG,
                        value);
}

static INLINE void odtu_dmx_reg_FORCE_CONFIG_REG_field_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_FORCE_CONFIG_REG_field_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_FORCE_CONFIG_REG_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_FORCE_CONFIG_REG,
                        mask,
                        PMC_ODTU_DMX_REG_FORCE_CONFIG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odtu_dmx_reg_FORCE_CONFIG_REG_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_FORCE_CONFIG_REG_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODTU_DMX_REG_FORCE_CONFIG_REG);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_FORCE_CONFIG_REG_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_write( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_write( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG,
                        value);
}

static INLINE void odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG,
                        mask,
                        PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_write( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_write( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG(N),
                        value);
}

static INLINE void odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_field_set", N, mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG(N),
                        mask,
                        PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odtu_dmx_reg_HO_CHANNEL_RESET_array_write( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_HO_CHANNEL_RESET_array_write( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_HO_CHANNEL_RESET_array_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_HO_CHANNEL_RESET(N),
                        value);
}

static INLINE void odtu_dmx_reg_HO_CHANNEL_RESET_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_HO_CHANNEL_RESET_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_HO_CHANNEL_RESET_array_field_set", N, mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_HO_CHANNEL_RESET(N),
                        mask,
                        PMC_ODTU_DMX_REG_HO_CHANNEL_RESET_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odtu_dmx_reg_HO_CHANNEL_RESET_array_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_HO_CHANNEL_RESET_array_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODTU_DMX_REG_HO_CHANNEL_RESET(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odtu_dmx_reg_HO_CHANNEL_RESET_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_write( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_write( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG(N),
                        value);
}

static INLINE void odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_field_set", N, mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG(N),
                        mask,
                        PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odtu_dmx_reg_NON_INTRUSIVE_MODE_array_write( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_NON_INTRUSIVE_MODE_array_write( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_NON_INTRUSIVE_MODE_array_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE(N),
                        value);
}

static INLINE void odtu_dmx_reg_NON_INTRUSIVE_MODE_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_NON_INTRUSIVE_MODE_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_NON_INTRUSIVE_MODE_array_field_set", N, mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE(N),
                        mask,
                        PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odtu_dmx_reg_NON_INTRUSIVE_MODE_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_NON_INTRUSIVE_MODE_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odtu_dmx_reg_NON_INTRUSIVE_MODE_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odtu_dmx_reg_LO_CHANNEL_RESET_array_write( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_LO_CHANNEL_RESET_array_write( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_LO_CHANNEL_RESET_array_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_LO_CHANNEL_RESET(N),
                        value);
}

static INLINE void odtu_dmx_reg_LO_CHANNEL_RESET_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_LO_CHANNEL_RESET_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_LO_CHANNEL_RESET_array_field_set", N, mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_LO_CHANNEL_RESET(N),
                        mask,
                        PMC_ODTU_DMX_REG_LO_CHANNEL_RESET_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odtu_dmx_reg_LO_CHANNEL_RESET_array_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_LO_CHANNEL_RESET_array_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODTU_DMX_REG_LO_CHANNEL_RESET(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odtu_dmx_reg_LO_CHANNEL_RESET_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odtu_dmx_reg_CONFIG_UPDATE_REG_array_write( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CONFIG_UPDATE_REG_array_write( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_CONFIG_UPDATE_REG_array_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG(N),
                        value);
}

static INLINE void odtu_dmx_reg_CONFIG_UPDATE_REG_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CONFIG_UPDATE_REG_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_CONFIG_UPDATE_REG_array_field_set", N, mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG(N),
                        mask,
                        PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odtu_dmx_reg_CONFIG_UPDATE_REG_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_CONFIG_UPDATE_REG_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odtu_dmx_reg_CONFIG_UPDATE_REG_array_read", reg_value, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CONFIG_UPDATE_REG_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                              odtu_dmx_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CONFIG_UPDATE_REG_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                              odtu_dmx_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_CONFIG_UPDATE_REG_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_DLOOMFI_INT_EN_write( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DLOOMFI_INT_EN_write( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_DLOOMFI_INT_EN_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_DLOOMFI_INT_EN,
                        value);
}

static INLINE void odtu_dmx_reg_DLOOMFI_INT_EN_field_set( odtu_dmx_buffer_t *b_ptr,
                                                          odtu_dmx_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DLOOMFI_INT_EN_field_set( odtu_dmx_buffer_t *b_ptr,
                                                          odtu_dmx_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_DLOOMFI_INT_EN_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_DLOOMFI_INT_EN,
                        mask,
                        PMC_ODTU_DMX_REG_DLOOMFI_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odtu_dmx_reg_DLOOMFI_INT_EN_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_DLOOMFI_INT_EN_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_DLOOMFI_INT_EN);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_DLOOMFI_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_OOM_OM_INT_EN_write( odtu_dmx_buffer_t *b_ptr,
                                                     odtu_dmx_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_OOM_OM_INT_EN_write( odtu_dmx_buffer_t *b_ptr,
                                                     odtu_dmx_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_OOM_OM_INT_EN_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_OOM_OM_INT_EN,
                        value);
}

static INLINE void odtu_dmx_reg_OOM_OM_INT_EN_field_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_OOM_OM_INT_EN_field_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_OOM_OM_INT_EN_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_OOM_OM_INT_EN,
                        mask,
                        PMC_ODTU_DMX_REG_OOM_OM_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odtu_dmx_reg_OOM_OM_INT_EN_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_OOM_OM_INT_EN_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_OOM_OM_INT_EN);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_OOM_OM_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_write( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_write( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN,
                        value);
}

static INLINE void odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN,
                        mask,
                        PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN(N),
                        mask,
                        PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                 odtu_dmx_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                 odtu_dmx_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_HAO_STAT_INT_EN(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_HAO_STAT_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_HAO_STAT_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_HAO_STAT_INT_EN_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_HAO_STAT_INT_EN(N),
                        mask,
                        PMC_ODTU_DMX_REG_HAO_STAT_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_HAO_STAT_INT_EN(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_HAO_STAT_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_HAO_STAT_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_HAO_STAT_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_HAO_STAT_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_DPLM_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DPLM_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_DPLM_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_DPLM_INT_EN_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_DPLM_INT_EN(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_DPLM_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                             odtu_dmx_handle_t *h_ptr,
                                                             UINT32 N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DPLM_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                             odtu_dmx_handle_t *h_ptr,
                                                             UINT32 N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_DPLM_INT_EN_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_DPLM_INT_EN(N),
                        mask,
                        PMC_ODTU_DMX_REG_DPLM_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_DPLM_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DPLM_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_DPLM_INT_EN_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_DPLM_INT_EN(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_DPLM_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_DPLM_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                          odtu_dmx_handle_t *h_ptr,
                                                          UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_DPLM_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                          odtu_dmx_handle_t *h_ptr,
                                                          UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_DPLM_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_DPLM_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_DMSIM_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 ofs,
                                                                UINT32 len,
                                                                UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DMSIM_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 ofs,
                                                                UINT32 len,
                                                                UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_DMSIM_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_DMSIM_INT_EN_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_DMSIM_INT_EN(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_DMSIM_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DMSIM_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_DMSIM_INT_EN_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_DMSIM_INT_EN(N),
                        mask,
                        PMC_ODTU_DMX_REG_DMSIM_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_DMSIM_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DMSIM_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_DMSIM_INT_EN_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_DMSIM_INT_EN(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_DMSIM_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_DMSIM_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_DMSIM_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_DMSIM_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_DMSIM_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN(N),
                        mask,
                        PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN(N),
                        mask,
                        PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN(N),
                        mask,
                        PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_CND_MAX_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CND_MAX_INT_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CND_MAX_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CND_MAX_INT_EN_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CND_MAX_INT_EN(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_CND_MAX_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CND_MAX_INT_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_CND_MAX_INT_EN_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_CND_MAX_INT_EN(N),
                        mask,
                        PMC_ODTU_DMX_REG_CND_MAX_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_CND_MAX_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CND_MAX_INT_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CND_MAX_INT_EN_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_CND_MAX_INT_EN(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CND_MAX_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_CND_MAX_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                             odtu_dmx_handle_t *h_ptr,
                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_CND_MAX_INT_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                             odtu_dmx_handle_t *h_ptr,
                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_CND_MAX_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_CND_MAX_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E(N),
                        mask,
                        PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN(N),
                        mask,
                        PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 ofs,
                                                                    UINT32 len,
                                                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 ofs,
                                                                    UINT32 len,
                                                                    UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN(N),
                        mask,
                        PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_field_set", mask, ofs, value );
    odtu_dmx_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN(N),
                        mask,
                        PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_read", reg_value);
    return reg_value;
}

static INLINE UINT32 odtu_dmx_reg_INTERRUPT_SUMMARY_1_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_INTERRUPT_SUMMARY_1_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_1);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_INTERRUPT_SUMMARY_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_INTERRUPT_SUMMARY_1_poll( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_INTERRUPT_SUMMARY_1_poll( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_INTERRUPT_SUMMARY_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_1,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE UINT32 odtu_dmx_reg_INTERRUPT_SUMMARY_2_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_INTERRUPT_SUMMARY_2_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_2);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_INTERRUPT_SUMMARY_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_INTERRUPT_SUMMARY_2_poll( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_INTERRUPT_SUMMARY_2_poll( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_INTERRUPT_SUMMARY_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_2,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_DLOOMFI_INT_write( odtu_dmx_buffer_t *b_ptr,
                                                   odtu_dmx_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DLOOMFI_INT_write( odtu_dmx_buffer_t *b_ptr,
                                                   odtu_dmx_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_DLOOMFI_INT_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_DLOOMFI_INT,
                        value);
}

static INLINE void odtu_dmx_reg_DLOOMFI_INT_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DLOOMFI_INT_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_DLOOMFI_INT_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_DLOOMFI_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odtu_dmx_reg_DLOOMFI_INT_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_DLOOMFI_INT_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_DLOOMFI_INT);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_DLOOMFI_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DLOOMFI_INT_poll( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DLOOMFI_INT_poll( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_DLOOMFI_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_DLOOMFI_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_OOM_IM_INT_write( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_OOM_IM_INT_write( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_OOM_IM_INT_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_OOM_IM_INT,
                        value);
}

static INLINE void odtu_dmx_reg_OOM_IM_INT_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_OOM_IM_INT_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_OOM_IM_INT_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_OOM_IM_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odtu_dmx_reg_OOM_IM_INT_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_OOM_IM_INT_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_OOM_IM_INT);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_OOM_IM_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_OOM_IM_INT_poll( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_OOM_IM_INT_poll( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_OOM_IM_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_OOM_IM_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_write( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_write( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_write", value );
    odtu_dmx_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT,
                        value);
}

static INLINE void odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                       odtu_dmx_handle_t *h_ptr,
                                                                                       UINT32 mask,
                                                                                       UINT32 ofs,
                                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                       odtu_dmx_handle_t *h_ptr,
                                                                                       UINT32 mask,
                                                                                       UINT32 ofs,
                                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                  odtu_dmx_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                  odtu_dmx_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                         odtu_dmx_handle_t *h_ptr,
                                                                         UINT32 ofs,
                                                                         UINT32 len,
                                                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                         odtu_dmx_handle_t *h_ptr,
                                                                         UINT32 ofs,
                                                                         UINT32 len,
                                                                         UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                       odtu_dmx_handle_t *h_ptr,
                                                                                       UINT32 N,
                                                                                       UINT32 mask,
                                                                                       UINT32 ofs,
                                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                       odtu_dmx_handle_t *h_ptr,
                                                                                       UINT32 N,
                                                                                       UINT32 mask,
                                                                                       UINT32 ofs,
                                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                  odtu_dmx_handle_t *h_ptr,
                                                                                  UINT32 N,
                                                                                  UINT32 mask,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                  odtu_dmx_handle_t *h_ptr,
                                                                                  UINT32 N,
                                                                                  UINT32 mask,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_HAO_STAT_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 ofs,
                                                                UINT32 len,
                                                                UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_HAO_STAT_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 ofs,
                                                                UINT32 len,
                                                                UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_HAO_STAT_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_HAO_STAT_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_HAO_STAT_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_HAO_STAT_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                              odtu_dmx_handle_t *h_ptr,
                                                                              UINT32 N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_HAO_STAT_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                              odtu_dmx_handle_t *h_ptr,
                                                                              UINT32 N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_HAO_STAT_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_HAO_STAT_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_HAO_STAT_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_HAO_STAT_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_HAO_STAT_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_HAO_STAT_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_HAO_STAT_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_HAO_STAT_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_HAO_STAT_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_HAO_STAT_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_HAO_STAT_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_HAO_STAT_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                         odtu_dmx_handle_t *h_ptr,
                                                                         UINT32 N,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_HAO_STAT_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                         odtu_dmx_handle_t *h_ptr,
                                                                         UINT32 N,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_HAO_STAT_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_HAO_STAT_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_DPLM_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 ofs,
                                                            UINT32 len,
                                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DPLM_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 ofs,
                                                            UINT32 len,
                                                            UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_DPLM_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_DPLM_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_DPLM_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_DPLM_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DPLM_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_DPLM_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_DPLM_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_DPLM_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DPLM_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_DPLM_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_DPLM_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_DPLM_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_DPLM_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_DPLM_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_DPLM_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_DPLM_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DPLM_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DPLM_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_DPLM_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_DPLM_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_DMSIM_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                             odtu_dmx_handle_t *h_ptr,
                                                             UINT32 ofs,
                                                             UINT32 len,
                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DMSIM_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                             odtu_dmx_handle_t *h_ptr,
                                                             UINT32 ofs,
                                                             UINT32 len,
                                                             UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_DMSIM_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_DMSIM_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_DMSIM_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_DMSIM_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                           odtu_dmx_handle_t *h_ptr,
                                                                           UINT32 N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DMSIM_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                           odtu_dmx_handle_t *h_ptr,
                                                                           UINT32 N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_DMSIM_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_DMSIM_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_DMSIM_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 ofs,
                                                            UINT32 len,
                                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DMSIM_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 ofs,
                                                            UINT32 len,
                                                            UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_DMSIM_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_DMSIM_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_DMSIM_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_DMSIM_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_DMSIM_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_DMSIM_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_DMSIM_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DMSIM_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 N,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DMSIM_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 N,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_DMSIM_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_DMSIM_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 ofs,
                                                                    UINT32 len,
                                                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 ofs,
                                                                    UINT32 len,
                                                                    UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                  odtu_dmx_handle_t *h_ptr,
                                                                                  UINT32 N,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                  odtu_dmx_handle_t *h_ptr,
                                                                                  UINT32 N,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_STS_SW_OVFLW_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_STS_SW_OVFLW_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_STS_SW_OVFLW_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_STS_SW_OVFLW_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_STS_SW_OVFLW_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 N,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_STS_SW_OVFLW_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 N,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_STS_SW_OVFLW_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                 odtu_dmx_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                 odtu_dmx_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_SSF_SF_RCVD_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_SSF_SF_RCVD_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_SSF_SF_RCVD_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_SSF_SF_RCVD_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_SSF_SF_RCVD_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                            odtu_dmx_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_SSF_SF_RCVD_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                            odtu_dmx_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_SSF_SF_RCVD_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                     odtu_dmx_handle_t *h_ptr,
                                                                                     UINT32 N,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                     odtu_dmx_handle_t *h_ptr,
                                                                                     UINT32 N,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                odtu_dmx_handle_t *h_ptr,
                                                                                UINT32 N,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                odtu_dmx_handle_t *h_ptr,
                                                                                UINT32 N,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                           odtu_dmx_handle_t *h_ptr,
                                                                                           UINT32 N,
                                                                                           UINT32 mask,
                                                                                           UINT32 ofs,
                                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                           odtu_dmx_handle_t *h_ptr,
                                                                                           UINT32 N,
                                                                                           UINT32 mask,
                                                                                           UINT32 ofs,
                                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                            odtu_dmx_handle_t *h_ptr,
                                                                            UINT32 ofs,
                                                                            UINT32 len,
                                                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                            odtu_dmx_handle_t *h_ptr,
                                                                            UINT32 ofs,
                                                                            UINT32 len,
                                                                            UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                      odtu_dmx_handle_t *h_ptr,
                                                                                      UINT32 N,
                                                                                      UINT32 mask,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                      odtu_dmx_handle_t *h_ptr,
                                                                                      UINT32 N,
                                                                                      UINT32 mask,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 ofs,
                                                                          UINT32 len,
                                                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 ofs,
                                                                          UINT32 len,
                                                                          UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                        odtu_dmx_handle_t *h_ptr,
                                                                                        UINT32 N,
                                                                                        UINT32 mask,
                                                                                        UINT32 ofs,
                                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                        odtu_dmx_handle_t *h_ptr,
                                                                                        UINT32 N,
                                                                                        UINT32 mask,
                                                                                        UINT32 ofs,
                                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                         odtu_dmx_handle_t *h_ptr,
                                                                         UINT32 ofs,
                                                                         UINT32 len,
                                                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                         odtu_dmx_handle_t *h_ptr,
                                                                         UINT32 ofs,
                                                                         UINT32 len,
                                                                         UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                   odtu_dmx_handle_t *h_ptr,
                                                                                   UINT32 N,
                                                                                   UINT32 mask,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                   odtu_dmx_handle_t *h_ptr,
                                                                                   UINT32 N,
                                                                                   UINT32 mask,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                    odtu_dmx_handle_t *h_ptr,
                                                                                    UINT32 N,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                    odtu_dmx_handle_t *h_ptr,
                                                                                    UINT32 N,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                               odtu_dmx_handle_t *h_ptr,
                                                                               UINT32 N,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                               odtu_dmx_handle_t *h_ptr,
                                                                               UINT32 N,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                      odtu_dmx_handle_t *h_ptr,
                                                                                      UINT32 N,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                      odtu_dmx_handle_t *h_ptr,
                                                                                      UINT32 N,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                 odtu_dmx_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                 odtu_dmx_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_write( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_write", 3, ofs, len);
    odtu_dmx_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT(ofs),
                          len,
                          value);

}

static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                      odtu_dmx_handle_t *h_ptr,
                                                                                      UINT32 N,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_action_on_write_field_set( odtu_dmx_buffer_t *b_ptr,
                                                                                      odtu_dmx_handle_t *h_ptr,
                                                                                      UINT32 N,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_action_on_write_field_set", mask, ofs, value );
    odtu_dmx_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT(N),
                                        mask,
                                        ofs,
                                        value);

}

static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 ofs,
                                                                       UINT32 len,
                                                                       UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                 odtu_dmx_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                 odtu_dmx_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE UINT32 odtu_dmx_reg_DLOOMFI_INT_VAL_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_DLOOMFI_INT_VAL_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_DLOOMFI_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_DLOOMFI_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DLOOMFI_INT_VAL_poll( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DLOOMFI_INT_VAL_poll( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_DLOOMFI_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_DLOOMFI_INT_VAL,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE UINT32 odtu_dmx_reg_OOM_IM_INT_VAL_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_OOM_IM_INT_VAL_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_OOM_IM_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_OOM_IM_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_OOM_IM_INT_VAL_poll( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_OOM_IM_INT_VAL_poll( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_OOM_IM_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_OOM_IM_INT_VAL,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_DPLM_INT_VAL_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DPLM_INT_VAL_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_DPLM_INT_VAL_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_DPLM_INT_VAL(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_DPLM_INT_VAL_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_DPLM_INT_VAL_array_read( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_DPLM_INT_VAL_array_read( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_DPLM_INT_VAL(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_DPLM_INT_VAL_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DPLM_INT_VAL_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                         odtu_dmx_handle_t *h_ptr,
                                                                         UINT32 N,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DPLM_INT_VAL_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                         odtu_dmx_handle_t *h_ptr,
                                                                         UINT32 N,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_DPLM_INT_VAL_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_DPLM_INT_VAL(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_DMSIM_INT_VAL_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 ofs,
                                                                UINT32 len,
                                                                UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_DMSIM_INT_VAL_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 ofs,
                                                                UINT32 len,
                                                                UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_DMSIM_INT_VAL_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_DMSIM_INT_VAL(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_DMSIM_INT_VAL_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_DMSIM_INT_VAL_array_read( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_DMSIM_INT_VAL_array_read( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_DMSIM_INT_VAL(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_DMSIM_INT_VAL_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DMSIM_INT_VAL_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 N,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_DMSIM_INT_VAL_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 N,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_DMSIM_INT_VAL_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_DMSIM_INT_VAL(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_burst_read( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_burst_read", ofs, len, 3 );
    odtu_dmx_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_read( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 N )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL(N));

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                odtu_dmx_handle_t *h_ptr,
                                                                                UINT32 N,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                odtu_dmx_handle_t *h_ptr,
                                                                                UINT32 N,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odtu_dmx_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE UINT32 odtu_dmx_reg_CONFIG_ADDR_ST_REG_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_reg_CONFIG_ADDR_ST_REG_read( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odtu_dmx_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_REG);

    IOLOG( "%s -> 0x%08x;", "odtu_dmx_reg_CONFIG_ADDR_ST_REG_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void odtu_dmx_field_FORCE_CONFIG_ADDR_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_FORCE_CONFIG_ADDR_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_FORCE_CONFIG_ADDR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odtu_dmx_field_FORCE_CONFIG_ADDR_set", value );

    /* (0x00000900 bits 1) field FORCE_CONFIG_ADDR of register PMC_ODTU_DMX_REG_FORCE_CONFIG_REG */
    odtu_dmx_reg_FORCE_CONFIG_REG_field_set( b_ptr,
                                             h_ptr,
                                             ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_ADDR_MSK,
                                             ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_ADDR_OFF,
                                             value);
}

static INLINE UINT32 odtu_dmx_field_FORCE_CONFIG_ADDR_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_FORCE_CONFIG_ADDR_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000900 bits 1) field FORCE_CONFIG_ADDR of register PMC_ODTU_DMX_REG_FORCE_CONFIG_REG */
    reg_value = odtu_dmx_reg_FORCE_CONFIG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_ADDR_MSK) >> ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_ADDR_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_FORCE_CONFIG_ADDR_get", value );

    return value;
}
static INLINE void odtu_dmx_field_FORCE_CONFIG_set( odtu_dmx_buffer_t *b_ptr,
                                                    odtu_dmx_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_FORCE_CONFIG_set( odtu_dmx_buffer_t *b_ptr,
                                                    odtu_dmx_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_FORCE_CONFIG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odtu_dmx_field_FORCE_CONFIG_set", value );

    /* (0x00000900 bits 0) field FORCE_CONFIG of register PMC_ODTU_DMX_REG_FORCE_CONFIG_REG */
    odtu_dmx_reg_FORCE_CONFIG_REG_field_set( b_ptr,
                                             h_ptr,
                                             ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_MSK,
                                             ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_OFF,
                                             value);
}

static INLINE UINT32 odtu_dmx_field_FORCE_CONFIG_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_FORCE_CONFIG_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000900 bits 0) field FORCE_CONFIG of register PMC_ODTU_DMX_REG_FORCE_CONFIG_REG */
    reg_value = odtu_dmx_reg_FORCE_CONFIG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_MSK) >> ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_FORCE_CONFIG_get", value );

    return value;
}
static INLINE void odtu_dmx_field_LO_CONFIG_ADDR_ST_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_LO_CONFIG_ADDR_ST_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_LO_CONFIG_ADDR_ST_ID_set", value, 127);
    IOLOG( "%s <= 0x%08x", "odtu_dmx_field_LO_CONFIG_ADDR_ST_ID_set", value );

    /* (0x00000a88 bits 14:8) field LO_CONFIG_ADDR_ST_ID of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG */
    odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_MSK,
                                                    ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_OFF,
                                                    value);
}

static INLINE UINT32 odtu_dmx_field_LO_CONFIG_ADDR_ST_ID_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_LO_CONFIG_ADDR_ST_ID_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000a88 bits 14:8) field LO_CONFIG_ADDR_ST_ID of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG */
    reg_value = odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_MSK) >> ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_LO_CONFIG_ADDR_ST_ID_get", value );

    return value;
}
static INLINE void odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000a88 bits 14:8) field LO_CONFIG_ADDR_ST_ID of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG */
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
        /* (0x00000a88 bits 14:8) field LO_CONFIG_ADDR_ST_ID of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG */
        odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_OFF + subfield_offset),
                                                        ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_get", stop_bit, 6 );
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
    /* (0x00000a88 bits 14:8) field LO_CONFIG_ADDR_ST_ID of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG */
    reg_value = odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_MSK)
                  >> ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_MSK, ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set", value, 127);
    IOLOG( "%s <= 0x%08x", "odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set", value );

    /* (0x00000a88 bits 6:0) field HO_CONFIG_ADDR_ST_ID of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG */
    odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_MSK,
                                                    ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_OFF,
                                                    value);
}

static INLINE UINT32 odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000a88 bits 6:0) field HO_CONFIG_ADDR_ST_ID of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG */
    reg_value = odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_MSK) >> ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_get", value );

    return value;
}
static INLINE void odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000a88 bits 6:0) field HO_CONFIG_ADDR_ST_ID of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG */
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
        /* (0x00000a88 bits 6:0) field HO_CONFIG_ADDR_ST_ID of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG */
        odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_OFF + subfield_offset),
                                                        ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_get", stop_bit, 6 );
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
    /* (0x00000a88 bits 6:0) field HO_CONFIG_ADDR_ST_ID of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG */
    reg_value = odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_MSK)
                  >> ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_MSK, ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size4
 * ==================================================================================
 */
static INLINE void odtu_dmx_field_DUMMY_SCHDL_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_DUMMY_SCHDL_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_DUMMY_SCHDL_ID_set", N, 3);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_DUMMY_SCHDL_ID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "odtu_dmx_field_DUMMY_SCHDL_ID_set", N, value );

    /* ((0x00000a94 + (N) * 0x4) bits 22:16) field DUMMY_SCHDL_ID of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
    odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_MSK,
                                                       ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_OFF,
                                                       value);
}

static INLINE UINT32 odtu_dmx_field_DUMMY_SCHDL_ID_get( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_DUMMY_SCHDL_ID_get( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_DUMMY_SCHDL_ID_get", N, 3);
    /* ((0x00000a94 + (N) * 0x4) bits 22:16) field DUMMY_SCHDL_ID of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
    reg_value = odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_MSK) >> ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odtu_dmx_field_DUMMY_SCHDL_ID_get", N, value );

    return value;
}
static INLINE void odtu_dmx_field_range_DUMMY_SCHDL_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_range_DUMMY_SCHDL_ID_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_range_DUMMY_SCHDL_ID_set", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_DUMMY_SCHDL_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_DUMMY_SCHDL_ID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_DUMMY_SCHDL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000a94 + (N) * 0x4) bits 22:16) field DUMMY_SCHDL_ID of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
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
        /* ((0x00000a94 + (N) * 0x4) bits 22:16) field DUMMY_SCHDL_ID of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
        odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_OFF + subfield_offset),
                                                           ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 odtu_dmx_field_range_DUMMY_SCHDL_ID_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_range_DUMMY_SCHDL_ID_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
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

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_range_DUMMY_SCHDL_ID_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_DUMMY_SCHDL_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_DUMMY_SCHDL_ID_get", stop_bit, 6 );
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
    /* ((0x00000a94 + (N) * 0x4) bits 22:16) field DUMMY_SCHDL_ID of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
    reg_value = odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_MSK)
                  >> ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_MSK, ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_DUMMY_SCHDL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odtu_dmx_field_DUMMY_SCHDL_CYCLE_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_DUMMY_SCHDL_CYCLE_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_DUMMY_SCHDL_CYCLE_set", N, 3);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_DUMMY_SCHDL_CYCLE_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "odtu_dmx_field_DUMMY_SCHDL_CYCLE_set", N, value );

    /* ((0x00000a94 + (N) * 0x4) bits 14:8) field DUMMY_SCHDL_CYCLE of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
    odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_MSK,
                                                       ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_OFF,
                                                       value);
}

static INLINE UINT32 odtu_dmx_field_DUMMY_SCHDL_CYCLE_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_DUMMY_SCHDL_CYCLE_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_DUMMY_SCHDL_CYCLE_get", N, 3);
    /* ((0x00000a94 + (N) * 0x4) bits 14:8) field DUMMY_SCHDL_CYCLE of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
    reg_value = odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_MSK) >> ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odtu_dmx_field_DUMMY_SCHDL_CYCLE_get", N, value );

    return value;
}
static INLINE void odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_set( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_set( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_set", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000a94 + (N) * 0x4) bits 14:8) field DUMMY_SCHDL_CYCLE of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
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
        /* ((0x00000a94 + (N) * 0x4) bits 14:8) field DUMMY_SCHDL_CYCLE of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
        odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           subfield_mask << (ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_OFF + subfield_offset),
                                                           ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_get( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_get( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
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

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_get", stop_bit, 6 );
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
    /* ((0x00000a94 + (N) * 0x4) bits 14:8) field DUMMY_SCHDL_CYCLE of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
    reg_value = odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_MSK)
                  >> ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_MSK, ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odtu_dmx_field_DUMMY_SCHDL_EN_set( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_DUMMY_SCHDL_EN_set( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_DUMMY_SCHDL_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_DUMMY_SCHDL_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odtu_dmx_field_DUMMY_SCHDL_EN_set", N, value );

    /* ((0x00000a94 + (N) * 0x4) bits 0) field DUMMY_SCHDL_EN of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
    odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_EN_MSK,
                                                       ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_EN_OFF,
                                                       value);
}

static INLINE UINT32 odtu_dmx_field_DUMMY_SCHDL_EN_get( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_DUMMY_SCHDL_EN_get( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_DUMMY_SCHDL_EN_get", N, 3);
    /* ((0x00000a94 + (N) * 0x4) bits 0) field DUMMY_SCHDL_EN of register PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG index N=0..3 */
    reg_value = odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_EN_MSK) >> ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odtu_dmx_field_DUMMY_SCHDL_EN_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void odtu_dmx_field_HO_CHNL_RSTB_set( odtu_dmx_buffer_t *b_ptr,
                                                    odtu_dmx_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_HO_CHNL_RSTB_set( odtu_dmx_buffer_t *b_ptr,
                                                    odtu_dmx_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_HO_CHNL_RSTB_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_HO_CHNL_RSTB_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odtu_dmx_field_HO_CHNL_RSTB_set", N, value );

    /* ((0x00000000 + (N) * 0x10) bits 0) field HO_CHNL_RSTB of register PMC_ODTU_DMX_REG_HO_CHANNEL_RESET index N=0..95 */
    odtu_dmx_reg_HO_CHANNEL_RESET_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   ODTU_DMX_REG_HO_CHANNEL_RESET_BIT_HO_CHNL_RSTB_MSK,
                                                   ODTU_DMX_REG_HO_CHANNEL_RESET_BIT_HO_CHNL_RSTB_OFF,
                                                   value);
}

static INLINE UINT32 odtu_dmx_field_HO_CHNL_RSTB_get( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_HO_CHNL_RSTB_get( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_HO_CHNL_RSTB_get", N, 95);
    /* ((0x00000000 + (N) * 0x10) bits 0) field HO_CHNL_RSTB of register PMC_ODTU_DMX_REG_HO_CHANNEL_RESET index N=0..95 */
    reg_value = odtu_dmx_reg_HO_CHANNEL_RESET_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & ODTU_DMX_REG_HO_CHANNEL_RESET_BIT_HO_CHNL_RSTB_MSK) >> ODTU_DMX_REG_HO_CHANNEL_RESET_BIT_HO_CHNL_RSTB_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odtu_dmx_field_HO_CHNL_RSTB_get", N, value );

    return value;
}
static INLINE void odtu_dmx_field_PASSTHRU_set( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_PASSTHRU_set( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_PASSTHRU_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_PASSTHRU_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odtu_dmx_field_PASSTHRU_set", N, value );

    /* ((0x00000004 + (N) * 0x10) bits 0) field PASSTHRU of register PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG index N=0..95 */
    odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       ODTU_DMX_REG_PASS_THROUGH_CFG_REG_BIT_PASSTHRU_MSK,
                                                       ODTU_DMX_REG_PASS_THROUGH_CFG_REG_BIT_PASSTHRU_OFF,
                                                       value);
}

static INLINE UINT32 odtu_dmx_field_PASSTHRU_get( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_PASSTHRU_get( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_PASSTHRU_get", N, 95);
    /* ((0x00000004 + (N) * 0x10) bits 0) field PASSTHRU of register PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG index N=0..95 */
    reg_value = odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & ODTU_DMX_REG_PASS_THROUGH_CFG_REG_BIT_PASSTHRU_MSK) >> ODTU_DMX_REG_PASS_THROUGH_CFG_REG_BIT_PASSTHRU_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odtu_dmx_field_PASSTHRU_get", N, value );

    return value;
}
static INLINE void odtu_dmx_field_NON_INTRUSVE_MODE_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_NON_INTRUSVE_MODE_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_NON_INTRUSVE_MODE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_NON_INTRUSVE_MODE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odtu_dmx_field_NON_INTRUSVE_MODE_set", N, value );

    /* ((0x00000008 + (N) * 0x10) bits 0) field NON_INTRUSVE_MODE of register PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE index N=0..95 */
    odtu_dmx_reg_NON_INTRUSIVE_MODE_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     ODTU_DMX_REG_NON_INTRUSIVE_MODE_BIT_NON_INTRUSVE_MODE_MSK,
                                                     ODTU_DMX_REG_NON_INTRUSIVE_MODE_BIT_NON_INTRUSVE_MODE_OFF,
                                                     value);
}

static INLINE UINT32 odtu_dmx_field_NON_INTRUSVE_MODE_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_NON_INTRUSVE_MODE_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_NON_INTRUSVE_MODE_get", N, 95);
    /* ((0x00000008 + (N) * 0x10) bits 0) field NON_INTRUSVE_MODE of register PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE index N=0..95 */
    reg_value = odtu_dmx_reg_NON_INTRUSIVE_MODE_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODTU_DMX_REG_NON_INTRUSIVE_MODE_BIT_NON_INTRUSVE_MODE_MSK) >> ODTU_DMX_REG_NON_INTRUSIVE_MODE_BIT_NON_INTRUSVE_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odtu_dmx_field_NON_INTRUSVE_MODE_get", N, value );

    return value;
}
static INLINE void odtu_dmx_field_LO_CHNL_RSTB_set( odtu_dmx_buffer_t *b_ptr,
                                                    odtu_dmx_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_LO_CHNL_RSTB_set( odtu_dmx_buffer_t *b_ptr,
                                                    odtu_dmx_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_LO_CHNL_RSTB_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_LO_CHNL_RSTB_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odtu_dmx_field_LO_CHNL_RSTB_set", N, value );

    /* ((0x00000600 + (N) * 0x4) bits 0) field LO_CHNL_RSTB of register PMC_ODTU_DMX_REG_LO_CHANNEL_RESET index N=0..95 */
    odtu_dmx_reg_LO_CHANNEL_RESET_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   ODTU_DMX_REG_LO_CHANNEL_RESET_BIT_LO_CHNL_RSTB_MSK,
                                                   ODTU_DMX_REG_LO_CHANNEL_RESET_BIT_LO_CHNL_RSTB_OFF,
                                                   value);
}

static INLINE UINT32 odtu_dmx_field_LO_CHNL_RSTB_get( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_LO_CHNL_RSTB_get( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_LO_CHNL_RSTB_get", N, 95);
    /* ((0x00000600 + (N) * 0x4) bits 0) field LO_CHNL_RSTB of register PMC_ODTU_DMX_REG_LO_CHANNEL_RESET index N=0..95 */
    reg_value = odtu_dmx_reg_LO_CHANNEL_RESET_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & ODTU_DMX_REG_LO_CHANNEL_RESET_BIT_LO_CHNL_RSTB_MSK) >> ODTU_DMX_REG_LO_CHANNEL_RESET_BIT_LO_CHNL_RSTB_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odtu_dmx_field_LO_CHNL_RSTB_get", N, value );

    return value;
}
static INLINE void odtu_dmx_field_CONFIG_MODE_set( odtu_dmx_buffer_t *b_ptr,
                                                   odtu_dmx_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_CONFIG_MODE_set( odtu_dmx_buffer_t *b_ptr,
                                                   odtu_dmx_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_CONFIG_MODE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_CONFIG_MODE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odtu_dmx_field_CONFIG_MODE_set", N, value );

    /* ((0x00000780 + (N) * 0x4) bits 1) field CONFIG_MODE of register PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG index N=0..95 */
    odtu_dmx_reg_CONFIG_UPDATE_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_MODE_MSK,
                                                    ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_MODE_OFF,
                                                    value);
}

static INLINE UINT32 odtu_dmx_field_CONFIG_MODE_get( odtu_dmx_buffer_t *b_ptr,
                                                     odtu_dmx_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_CONFIG_MODE_get( odtu_dmx_buffer_t *b_ptr,
                                                     odtu_dmx_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_CONFIG_MODE_get", N, 95);
    /* ((0x00000780 + (N) * 0x4) bits 1) field CONFIG_MODE of register PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG index N=0..95 */
    reg_value = odtu_dmx_reg_CONFIG_UPDATE_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_MODE_MSK) >> ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odtu_dmx_field_CONFIG_MODE_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_CONFIG_MODE_poll( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_CONFIG_MODE_poll( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "odtu_dmx_field_CONFIG_MODE_poll", N, value );

    /* ((0x00000780 + (N) * 0x4) bits 1) field CONFIG_MODE of register PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG index N=0..95 */
    return odtu_dmx_reg_CONFIG_UPDATE_REG_array_poll( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_MODE_MSK,
                                                      (value<<ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_MODE_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge_N_size96
 * ==================================================================================
 */
static INLINE void odtu_dmx_field_CONFIG_UPDATE_set( odtu_dmx_buffer_t *b_ptr,
                                                     odtu_dmx_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_CONFIG_UPDATE_set( odtu_dmx_buffer_t *b_ptr,
                                                     odtu_dmx_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_CONFIG_UPDATE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_CONFIG_UPDATE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odtu_dmx_field_CONFIG_UPDATE_set", N, value );

    /* ((0x00000780 + (N) * 0x4) bits 0) field CONFIG_UPDATE of register PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG index N=0..95 */
    odtu_dmx_reg_CONFIG_UPDATE_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_UPDATE_MSK,
                                                    ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_UPDATE_OFF,
                                                    value);
}

static INLINE UINT32 odtu_dmx_field_CONFIG_UPDATE_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_CONFIG_UPDATE_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odtu_dmx_field_CONFIG_UPDATE_get", N, 95);
    /* ((0x00000780 + (N) * 0x4) bits 0) field CONFIG_UPDATE of register PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG index N=0..95 */
    reg_value = odtu_dmx_reg_CONFIG_UPDATE_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_UPDATE_MSK) >> ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_UPDATE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odtu_dmx_field_CONFIG_UPDATE_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_CONFIG_UPDATE_poll( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_CONFIG_UPDATE_poll( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "odtu_dmx_field_CONFIG_UPDATE_poll", N, value );

    /* ((0x00000780 + (N) * 0x4) bits 0) field CONFIG_UPDATE of register PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG index N=0..95 */
    return odtu_dmx_reg_CONFIG_UPDATE_REG_array_poll( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_UPDATE_MSK,
                                                      (value<<ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_UPDATE_OFF),
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
static INLINE void odtu_dmx_field_DLOOMFI_E_set( odtu_dmx_buffer_t *b_ptr,
                                                 odtu_dmx_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_DLOOMFI_E_set( odtu_dmx_buffer_t *b_ptr,
                                                 odtu_dmx_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_DLOOMFI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odtu_dmx_field_DLOOMFI_E_set", value );

    /* (0x00000940 bits 0) field DLOOMFI_E of register PMC_ODTU_DMX_REG_DLOOMFI_INT_EN */
    odtu_dmx_reg_DLOOMFI_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           ODTU_DMX_REG_DLOOMFI_INT_EN_BIT_DLOOMFI_E_MSK,
                                           ODTU_DMX_REG_DLOOMFI_INT_EN_BIT_DLOOMFI_E_OFF,
                                           value);
}

static INLINE UINT32 odtu_dmx_field_DLOOMFI_E_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_DLOOMFI_E_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000940 bits 0) field DLOOMFI_E of register PMC_ODTU_DMX_REG_DLOOMFI_INT_EN */
    reg_value = odtu_dmx_reg_DLOOMFI_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODTU_DMX_REG_DLOOMFI_INT_EN_BIT_DLOOMFI_E_MSK) >> ODTU_DMX_REG_DLOOMFI_INT_EN_BIT_DLOOMFI_E_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_DLOOMFI_E_get", value );

    return value;
}
static INLINE void odtu_dmx_field_OOM_IM_E_set( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_OOM_IM_E_set( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_OOM_IM_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odtu_dmx_field_OOM_IM_E_set", value );

    /* (0x00000944 bits 0) field OOM_IM_E of register PMC_ODTU_DMX_REG_OOM_OM_INT_EN */
    odtu_dmx_reg_OOM_OM_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODTU_DMX_REG_OOM_OM_INT_EN_BIT_OOM_IM_E_MSK,
                                          ODTU_DMX_REG_OOM_OM_INT_EN_BIT_OOM_IM_E_OFF,
                                          value);
}

static INLINE UINT32 odtu_dmx_field_OOM_IM_E_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_OOM_IM_E_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000944 bits 0) field OOM_IM_E of register PMC_ODTU_DMX_REG_OOM_OM_INT_EN */
    reg_value = odtu_dmx_reg_OOM_OM_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODTU_DMX_REG_OOM_OM_INT_EN_BIT_OOM_IM_E_MSK) >> ODTU_DMX_REG_OOM_OM_INT_EN_BIT_OOM_IM_E_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_OOM_IM_E_get", value );

    return value;
}
static INLINE void odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_E_set( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_E_set( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_E_set", value );

    /* (0x00000958 bits 0) field RCOH_TRNSFR_SHDW_DNE_E of register PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN */
    odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_field_set( b_ptr,
                                                      h_ptr,
                                                      ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN_BIT_RCOH_TRNSFR_SHDW_DNE_E_MSK,
                                                      ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN_BIT_RCOH_TRNSFR_SHDW_DNE_E_OFF,
                                                      value);
}

static INLINE UINT32 odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_E_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_E_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000958 bits 0) field RCOH_TRNSFR_SHDW_DNE_E of register PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN */
    reg_value = odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN_BIT_RCOH_TRNSFR_SHDW_DNE_E_MSK) >> ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN_BIT_RCOH_TRNSFR_SHDW_DNE_E_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable_N_burstsize3
 * ==================================================================================
 */
static INLINE void odtu_dmx_lfield_CONFIG_UPDATE_DNE_E_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CONFIG_UPDATE_DNE_E_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CONFIG_UPDATE_DNE_E_set");
    odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_write( b_ptr,
                                                         h_ptr,
                                                         0,
                                                         3,
                                                         value);
}

static INLINE void odtu_dmx_lfield_CONFIG_UPDATE_DNE_E_get( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CONFIG_UPDATE_DNE_E_get( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CONFIG_UPDATE_DNE_E_get");
    odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_read( b_ptr,
                                                        h_ptr,
                                                        0,
                                                        3,
                                                        (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_field_set( b_ptr,
                                                       h_ptr,
                                                       word_number,
                                                       mask,
                                                       field_ofs,
                                                       value);
}

static INLINE UINT32 odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_read( b_ptr,
                                                              h_ptr,
                                                              word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                   odtu_dmx_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                   odtu_dmx_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_set", start_bit, stop_bit);
    return odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_poll( b_ptr,
                                                         h_ptr,
                                                         word_number,
                                                         mask,
                                                         ( value << field_ofs),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_HAO_STATUS_AVL_E_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_HAO_STATUS_AVL_E_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_HAO_STATUS_AVL_E_set");
    odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_write( b_ptr,
                                                    h_ptr,
                                                    0,
                                                    3,
                                                    value);
}

static INLINE void odtu_dmx_lfield_HAO_STATUS_AVL_E_get( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_HAO_STATUS_AVL_E_get( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_HAO_STATUS_AVL_E_get");
    odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_read( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   3,
                                                   (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_HAO_STATUS_AVL_E_set( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_HAO_STATUS_AVL_E_set( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_HAO_STATUS_AVL_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_HAO_STATUS_AVL_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_HAO_STAT_INT_EN_array_field_set( b_ptr,
                                                  h_ptr,
                                                  word_number,
                                                  mask,
                                                  field_ofs,
                                                  value);
}

static INLINE UINT32 odtu_dmx_lfield_range_HAO_STATUS_AVL_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_HAO_STATUS_AVL_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_HAO_STATUS_AVL_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_HAO_STAT_INT_EN_array_read( b_ptr,
                                                         h_ptr,
                                                         word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_HAO_STATUS_AVL_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odtu_dmx_lfield_DPLM_E_set( odtu_dmx_buffer_t *b_ptr,
                                               odtu_dmx_handle_t *h_ptr,
                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_DPLM_E_set( odtu_dmx_buffer_t *b_ptr,
                                               odtu_dmx_handle_t *h_ptr,
                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_DPLM_E_set");
    odtu_dmx_reg_DPLM_INT_EN_array_burst_write( b_ptr,
                                                h_ptr,
                                                0,
                                                3,
                                                value);
}

static INLINE void odtu_dmx_lfield_DPLM_E_get( odtu_dmx_buffer_t *b_ptr,
                                               odtu_dmx_handle_t *h_ptr,
                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_DPLM_E_get( odtu_dmx_buffer_t *b_ptr,
                                               odtu_dmx_handle_t *h_ptr,
                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_DPLM_E_get");
    odtu_dmx_reg_DPLM_INT_EN_array_burst_read( b_ptr,
                                               h_ptr,
                                               0,
                                               3,
                                               (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_DPLM_E_set( odtu_dmx_buffer_t *b_ptr,
                                                     odtu_dmx_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_DPLM_E_set( odtu_dmx_buffer_t *b_ptr,
                                                     odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DPLM_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_DPLM_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_DPLM_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              word_number,
                                              mask,
                                              field_ofs,
                                              value);
}

static INLINE UINT32 odtu_dmx_lfield_range_DPLM_E_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_DPLM_E_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DPLM_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_DPLM_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_DPLM_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odtu_dmx_lfield_DMSIM_E_set( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_DMSIM_E_set( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_DMSIM_E_set");
    odtu_dmx_reg_DMSIM_INT_EN_array_burst_write( b_ptr,
                                                 h_ptr,
                                                 0,
                                                 3,
                                                 value);
}

static INLINE void odtu_dmx_lfield_DMSIM_E_get( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_DMSIM_E_get( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_DMSIM_E_get");
    odtu_dmx_reg_DMSIM_INT_EN_array_burst_read( b_ptr,
                                                h_ptr,
                                                0,
                                                3,
                                                (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_DMSIM_E_set( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_DMSIM_E_set( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_DMSIM_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_DMSIM_INT_EN_array_field_set( b_ptr,
                                               h_ptr,
                                               word_number,
                                               mask,
                                               field_ofs,
                                               value);
}

static INLINE UINT32 odtu_dmx_lfield_range_DMSIM_E_get( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_DMSIM_E_get( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_DMSIM_INT_EN_array_read( b_ptr,
                                                      h_ptr,
                                                      word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_DMSIM_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odtu_dmx_lfield_STS_SW_OVFLW_E_set( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_STS_SW_OVFLW_E_set( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_STS_SW_OVFLW_E_set");
    odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_write( b_ptr,
                                                        h_ptr,
                                                        0,
                                                        3,
                                                        value);
}

static INLINE void odtu_dmx_lfield_STS_SW_OVFLW_E_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_STS_SW_OVFLW_E_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_STS_SW_OVFLW_E_get");
    odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_read( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_STS_SW_OVFLW_E_set( odtu_dmx_buffer_t *b_ptr,
                                                             odtu_dmx_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_STS_SW_OVFLW_E_set( odtu_dmx_buffer_t *b_ptr,
                                                             odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_STS_SW_OVFLW_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_STS_SW_OVFLW_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_field_set( b_ptr,
                                                      h_ptr,
                                                      word_number,
                                                      mask,
                                                      field_ofs,
                                                      value);
}

static INLINE UINT32 odtu_dmx_lfield_range_STS_SW_OVFLW_E_get( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_STS_SW_OVFLW_E_get( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_STS_SW_OVFLW_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_read( b_ptr,
                                                             h_ptr,
                                                             word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_STS_SW_OVFLW_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odtu_dmx_lfield_SSF_SF_RCVD_E_set( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_SSF_SF_RCVD_E_set( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_SSF_SF_RCVD_E_set");
    odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void odtu_dmx_lfield_SSF_SF_RCVD_E_get( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_SSF_SF_RCVD_E_get( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_SSF_SF_RCVD_E_get");
    odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_SSF_SF_RCVD_E_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_SSF_SF_RCVD_E_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_SSF_SF_RCVD_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_field_set( b_ptr,
                                                     h_ptr,
                                                     word_number,
                                                     mask,
                                                     field_ofs,
                                                     value);
}

static INLINE UINT32 odtu_dmx_lfield_range_SSF_SF_RCVD_E_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_SSF_SF_RCVD_E_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_SSF_SF_RCVD_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odtu_dmx_lfield_CM_OUT_LIMIT_E_set( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CM_OUT_LIMIT_E_set( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CM_OUT_LIMIT_E_set");
    odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void odtu_dmx_lfield_CM_OUT_LIMIT_E_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CM_OUT_LIMIT_E_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CM_OUT_LIMIT_E_get");
    odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_CM_OUT_LIMIT_E_set( odtu_dmx_buffer_t *b_ptr,
                                                             odtu_dmx_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_CM_OUT_LIMIT_E_set( odtu_dmx_buffer_t *b_ptr,
                                                             odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_OUT_LIMIT_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CM_OUT_LIMIT_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_field_set( b_ptr,
                                                     h_ptr,
                                                     word_number,
                                                     mask,
                                                     field_ofs,
                                                     value);
}

static INLINE UINT32 odtu_dmx_lfield_range_CM_OUT_LIMIT_E_get( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_CM_OUT_LIMIT_E_get( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_OUT_LIMIT_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CM_OUT_LIMIT_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odtu_dmx_lfield_CND_MAX_E_set( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CND_MAX_E_set( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CND_MAX_E_set");
    odtu_dmx_reg_CND_MAX_INT_EN_array_burst_write( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   3,
                                                   value);
}

static INLINE void odtu_dmx_lfield_CND_MAX_E_get( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CND_MAX_E_get( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CND_MAX_E_get");
    odtu_dmx_reg_CND_MAX_INT_EN_array_burst_read( b_ptr,
                                                  h_ptr,
                                                  0,
                                                  3,
                                                  (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_CND_MAX_E_set( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_CND_MAX_E_set( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CND_MAX_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CND_MAX_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_CND_MAX_INT_EN_array_field_set( b_ptr,
                                                 h_ptr,
                                                 word_number,
                                                 mask,
                                                 field_ofs,
                                                 value);
}

static INLINE UINT32 odtu_dmx_lfield_range_CND_MAX_E_get( odtu_dmx_buffer_t *b_ptr,
                                                          odtu_dmx_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_CND_MAX_E_get( odtu_dmx_buffer_t *b_ptr,
                                                          odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CND_MAX_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_CND_MAX_INT_EN_array_read( b_ptr,
                                                        h_ptr,
                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CND_MAX_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_E_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_E_set( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_E_set");
    odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_write( b_ptr,
                                                        h_ptr,
                                                        0,
                                                        3,
                                                        value);
}

static INLINE void odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_E_get( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_E_get( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_E_get");
    odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_read( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_E_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_E_set( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_field_set( b_ptr,
                                                      h_ptr,
                                                      word_number,
                                                      mask,
                                                      field_ofs,
                                                      value);
}

static INLINE UINT32 odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_read( b_ptr,
                                                             h_ptr,
                                                             word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odtu_dmx_lfield_CM_CND_CRC_ERR_E_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CM_CND_CRC_ERR_E_set( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CM_CND_CRC_ERR_E_set");
    odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_write( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      value);
}

static INLINE void odtu_dmx_lfield_CM_CND_CRC_ERR_E_get( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CM_CND_CRC_ERR_E_get( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CM_CND_CRC_ERR_E_get");
    odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_read( b_ptr,
                                                     h_ptr,
                                                     0,
                                                     3,
                                                     (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_CM_CND_CRC_ERR_E_set( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_CM_CND_CRC_ERR_E_set( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_field_set( b_ptr,
                                                    h_ptr,
                                                    word_number,
                                                    mask,
                                                    field_ofs,
                                                    value);
}

static INLINE UINT32 odtu_dmx_lfield_range_CM_CND_CRC_ERR_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_CM_CND_CRC_ERR_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_read( b_ptr,
                                                           h_ptr,
                                                           word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odtu_dmx_lfield_ERLY_PSERV_EXPRY_E_set( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_ERLY_PSERV_EXPRY_E_set( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_ERLY_PSERV_EXPRY_E_set");
    odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_write( b_ptr,
                                                        h_ptr,
                                                        0,
                                                        3,
                                                        value);
}

static INLINE void odtu_dmx_lfield_ERLY_PSERV_EXPRY_E_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_ERLY_PSERV_EXPRY_E_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_ERLY_PSERV_EXPRY_E_get");
    odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_read( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_E_set( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_E_set( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_field_set( b_ptr,
                                                      h_ptr,
                                                      word_number,
                                                      mask,
                                                      field_ofs,
                                                      value);
}

static INLINE UINT32 odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_read( b_ptr,
                                                             h_ptr,
                                                             word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odtu_dmx_lfield_LATE_PSERV_EXPRY_E_set( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_LATE_PSERV_EXPRY_E_set( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_LATE_PSERV_EXPRY_E_set");
    odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_write( b_ptr,
                                                        h_ptr,
                                                        0,
                                                        3,
                                                        value);
}

static INLINE void odtu_dmx_lfield_LATE_PSERV_EXPRY_E_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_LATE_PSERV_EXPRY_E_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_LATE_PSERV_EXPRY_E_get");
    odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_read( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_LATE_PSERV_EXPRY_E_set( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_LATE_PSERV_EXPRY_E_set( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_E_set", start_bit, stop_bit, value);
    odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_field_set( b_ptr,
                                                      h_ptr,
                                                      word_number,
                                                      mask,
                                                      field_ofs,
                                                      value);
}

static INLINE UINT32 odtu_dmx_lfield_range_LATE_PSERV_EXPRY_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_LATE_PSERV_EXPRY_E_get( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_E_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_read( b_ptr,
                                                             h_ptr,
                                                             word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_E_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_summary
 * ==================================================================================
 */
static INLINE UINT32 odtu_dmx_field_INTR_SUMMARY_1_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_INTR_SUMMARY_1_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000a80 bits 31:0) bits 0:31 use field INTR_SUMMARY_1 of register PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_1 */
    reg_value = odtu_dmx_reg_INTERRUPT_SUMMARY_1_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_MSK) >> ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_INTR_SUMMARY_1_get", value );

    return value;
}
static INLINE UINT32 odtu_dmx_field_range_INTR_SUMMARY_1_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_range_INTR_SUMMARY_1_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_INTR_SUMMARY_1_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_INTR_SUMMARY_1_get", stop_bit, 31 );
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
    /* (0x00000a80 bits 31:0) bits 0:31 use field INTR_SUMMARY_1 of register PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_1 */
    reg_value = odtu_dmx_reg_INTERRUPT_SUMMARY_1_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_MSK)
                  >> ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_MSK, ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_INTR_SUMMARY_1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_range_INTR_SUMMARY_1_poll( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_range_INTR_SUMMARY_1_poll( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_INTR_SUMMARY_1_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_INTR_SUMMARY_1_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_INTR_SUMMARY_1_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000a80 bits 31:0) bits 0:31 use field INTR_SUMMARY_1 of register PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_1 */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000a80 bits 31:0) bits 0:31 use field INTR_SUMMARY_1 of register PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_1 */
        return odtu_dmx_reg_INTERRUPT_SUMMARY_1_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_OFF + subfield_offset),
                                                      value << (ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_INTR_SUMMARY_1_poll( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_INTR_SUMMARY_1_poll( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odtu_dmx_field_INTR_SUMMARY_1_poll", value );

    /* (0x00000a80 bits 31:0) bits 0:31 use field INTR_SUMMARY_1 of register PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_1 */
    return odtu_dmx_reg_INTERRUPT_SUMMARY_1_poll( b_ptr,
                                                  h_ptr,
                                                  ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_MSK,
                                                  (value<<ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE UINT32 odtu_dmx_field_INTR_SUMMARY_2_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_INTR_SUMMARY_2_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000a84 bits 6:0) bits 0:6 use field INTR_SUMMARY_2 of register PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_2 */
    reg_value = odtu_dmx_reg_INTERRUPT_SUMMARY_2_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_MSK) >> ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_INTR_SUMMARY_2_get", value );

    return value;
}
static INLINE UINT32 odtu_dmx_field_range_INTR_SUMMARY_2_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_range_INTR_SUMMARY_2_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_INTR_SUMMARY_2_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_INTR_SUMMARY_2_get", stop_bit, 6 );
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
    /* (0x00000a84 bits 6:0) bits 0:6 use field INTR_SUMMARY_2 of register PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_2 */
    reg_value = odtu_dmx_reg_INTERRUPT_SUMMARY_2_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_MSK)
                  >> ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_MSK, ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_INTR_SUMMARY_2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_range_INTR_SUMMARY_2_poll( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_range_INTR_SUMMARY_2_poll( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odtu_dmx_field_range_INTR_SUMMARY_2_poll", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odtu_dmx_field_range_INTR_SUMMARY_2_poll", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odtu_dmx_field_range_INTR_SUMMARY_2_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000a84 bits 6:0) bits 0:6 use field INTR_SUMMARY_2 of register PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_2 */
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
        if (stop_bit < 6) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 6;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000a84 bits 6:0) bits 0:6 use field INTR_SUMMARY_2 of register PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_2 */
        return odtu_dmx_reg_INTERRUPT_SUMMARY_2_poll( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_OFF + subfield_offset),
                                                      value << (ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_OFF + subfield_offset),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_INTR_SUMMARY_2_poll( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_INTR_SUMMARY_2_poll( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odtu_dmx_field_INTR_SUMMARY_2_poll", value );

    /* (0x00000a84 bits 6:0) bits 0:6 use field INTR_SUMMARY_2 of register PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_2 */
    return odtu_dmx_reg_INTERRUPT_SUMMARY_2_poll( b_ptr,
                                                  h_ptr,
                                                  ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_MSK,
                                                  (value<<ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void odtu_dmx_field_DLOOMFI_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                          odtu_dmx_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_DLOOMFI_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                          odtu_dmx_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_DLOOMFI_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odtu_dmx_field_DLOOMFI_I_set_to_clear", value );

    /* (0x000009e0 bits 0) field DLOOMFI_I of register PMC_ODTU_DMX_REG_DLOOMFI_INT */
    odtu_dmx_reg_DLOOMFI_INT_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        ODTU_DMX_REG_DLOOMFI_INT_BIT_DLOOMFI_I_MSK,
                                                        ODTU_DMX_REG_DLOOMFI_INT_BIT_DLOOMFI_I_OFF,
                                                        value);
}

static INLINE UINT32 odtu_dmx_field_DLOOMFI_I_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_DLOOMFI_I_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000009e0 bits 0) field DLOOMFI_I of register PMC_ODTU_DMX_REG_DLOOMFI_INT */
    reg_value = odtu_dmx_reg_DLOOMFI_INT_read(  b_ptr, h_ptr);
    value = (reg_value & ODTU_DMX_REG_DLOOMFI_INT_BIT_DLOOMFI_I_MSK) >> ODTU_DMX_REG_DLOOMFI_INT_BIT_DLOOMFI_I_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_DLOOMFI_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_DLOOMFI_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_DLOOMFI_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odtu_dmx_field_DLOOMFI_I_poll", value );

    /* (0x000009e0 bits 0) field DLOOMFI_I of register PMC_ODTU_DMX_REG_DLOOMFI_INT */
    return odtu_dmx_reg_DLOOMFI_INT_poll( b_ptr,
                                          h_ptr,
                                          ODTU_DMX_REG_DLOOMFI_INT_BIT_DLOOMFI_I_MSK,
                                          (value<<ODTU_DMX_REG_DLOOMFI_INT_BIT_DLOOMFI_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_field_OOM_IM_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_OOM_IM_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_OOM_IM_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odtu_dmx_field_OOM_IM_I_set_to_clear", value );

    /* (0x000009e4 bits 0) field OOM_IM_I of register PMC_ODTU_DMX_REG_OOM_IM_INT */
    odtu_dmx_reg_OOM_IM_INT_action_on_write_field_set( b_ptr,
                                                       h_ptr,
                                                       ODTU_DMX_REG_OOM_IM_INT_BIT_OOM_IM_I_MSK,
                                                       ODTU_DMX_REG_OOM_IM_INT_BIT_OOM_IM_I_OFF,
                                                       value);
}

static INLINE UINT32 odtu_dmx_field_OOM_IM_I_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_OOM_IM_I_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000009e4 bits 0) field OOM_IM_I of register PMC_ODTU_DMX_REG_OOM_IM_INT */
    reg_value = odtu_dmx_reg_OOM_IM_INT_read(  b_ptr, h_ptr);
    value = (reg_value & ODTU_DMX_REG_OOM_IM_INT_BIT_OOM_IM_I_MSK) >> ODTU_DMX_REG_OOM_IM_INT_BIT_OOM_IM_I_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_OOM_IM_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_OOM_IM_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_OOM_IM_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odtu_dmx_field_OOM_IM_I_poll", value );

    /* (0x000009e4 bits 0) field OOM_IM_I of register PMC_ODTU_DMX_REG_OOM_IM_INT */
    return odtu_dmx_reg_OOM_IM_INT_poll( b_ptr,
                                         h_ptr,
                                         ODTU_DMX_REG_OOM_IM_INT_BIT_OOM_IM_I_MSK,
                                         (value<<ODTU_DMX_REG_OOM_IM_INT_BIT_OOM_IM_I_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_set_to_clear", value );

    /* (0x000009f8 bits 0) field RCOH_TRNSFR_SHDW_DNE_I of register PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT */
    odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT_BIT_RCOH_TRNSFR_SHDW_DNE_I_MSK,
                                                                        ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT_BIT_RCOH_TRNSFR_SHDW_DNE_I_OFF,
                                                                        value);
}

static INLINE UINT32 odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000009f8 bits 0) field RCOH_TRNSFR_SHDW_DNE_I of register PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT */
    reg_value = odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT_BIT_RCOH_TRNSFR_SHDW_DNE_I_MSK) >> ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT_BIT_RCOH_TRNSFR_SHDW_DNE_I_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                               odtu_dmx_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                               odtu_dmx_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_poll", value );

    /* (0x000009f8 bits 0) field RCOH_TRNSFR_SHDW_DNE_I of register PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT */
    return odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_poll( b_ptr,
                                                          h_ptr,
                                                          ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT_BIT_RCOH_TRNSFR_SHDW_DNE_I_MSK,
                                                          (value<<ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT_BIT_RCOH_TRNSFR_SHDW_DNE_I_OFF),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync_N_burstsize3
 * ==================================================================================
 */
static INLINE void odtu_dmx_lfield_CONFIG_UPDATE_DNE_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CONFIG_UPDATE_DNE_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CONFIG_UPDATE_DNE_I_set_to_clear");
    odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_write( b_ptr,
                                                          h_ptr,
                                                          0,
                                                          3,
                                                          value);
}

static INLINE void odtu_dmx_lfield_CONFIG_UPDATE_DNE_I_get( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CONFIG_UPDATE_DNE_I_get( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CONFIG_UPDATE_DNE_I_get");
    odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_read( b_ptr,
                                                         h_ptr,
                                                         0,
                                                         3,
                                                         (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                           odtu_dmx_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                           odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        word_number,
                                                                        mask,
                                                                        field_ofs,
                                                                        value);
}

static INLINE UINT32 odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_read( b_ptr,
                                                               h_ptr,
                                                               word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                   odtu_dmx_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                   odtu_dmx_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_poll( b_ptr,
                                                          h_ptr,
                                                          word_number,
                                                          mask,
                                                          ( value << field_ofs),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_HAO_STATUS_AVL_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_HAO_STATUS_AVL_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_HAO_STATUS_AVL_I_set_to_clear");
    odtu_dmx_reg_HAO_STAT_INT_array_burst_write( b_ptr,
                                                 h_ptr,
                                                 0,
                                                 3,
                                                 value);
}

static INLINE void odtu_dmx_lfield_HAO_STATUS_AVL_I_get( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_HAO_STATUS_AVL_I_get( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_HAO_STATUS_AVL_I_get");
    odtu_dmx_reg_HAO_STAT_INT_array_burst_read( b_ptr,
                                                h_ptr,
                                                0,
                                                3,
                                                (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_HAO_STATUS_AVL_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_HAO_STATUS_AVL_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_HAO_STATUS_AVL_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_HAO_STATUS_AVL_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_HAO_STAT_INT_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               word_number,
                                                               mask,
                                                               field_ofs,
                                                               value);
}

static INLINE UINT32 odtu_dmx_lfield_range_HAO_STATUS_AVL_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_HAO_STATUS_AVL_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_HAO_STATUS_AVL_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_HAO_STAT_INT_array_read( b_ptr,
                                                      h_ptr,
                                                      word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_HAO_STATUS_AVL_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_HAO_STATUS_AVL_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                odtu_dmx_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_HAO_STATUS_AVL_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                odtu_dmx_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_HAO_STATUS_AVL_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_HAO_STATUS_AVL_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_HAO_STAT_INT_array_poll( b_ptr,
                                                 h_ptr,
                                                 word_number,
                                                 mask,
                                                 ( value << field_ofs),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_DPLM_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_DPLM_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_DPLM_I_set_to_clear");
    odtu_dmx_reg_DPLM_INT_array_burst_write( b_ptr,
                                             h_ptr,
                                             0,
                                             3,
                                             value);
}

static INLINE void odtu_dmx_lfield_DPLM_I_get( odtu_dmx_buffer_t *b_ptr,
                                               odtu_dmx_handle_t *h_ptr,
                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_DPLM_I_get( odtu_dmx_buffer_t *b_ptr,
                                               odtu_dmx_handle_t *h_ptr,
                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_DPLM_I_get");
    odtu_dmx_reg_DPLM_INT_array_burst_read( b_ptr,
                                            h_ptr,
                                            0,
                                            3,
                                            (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_DPLM_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_DPLM_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DPLM_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_DPLM_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_DPLM_INT_array_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           word_number,
                                                           mask,
                                                           field_ofs,
                                                           value);
}

static INLINE UINT32 odtu_dmx_lfield_range_DPLM_I_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_DPLM_I_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DPLM_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_DPLM_INT_array_read( b_ptr,
                                                  h_ptr,
                                                  word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_DPLM_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DPLM_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DPLM_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DPLM_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_DPLM_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_DPLM_INT_array_poll( b_ptr,
                                             h_ptr,
                                             word_number,
                                             mask,
                                             ( value << field_ofs),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_DMSIM_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_DMSIM_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_DMSIM_I_set_to_clear");
    odtu_dmx_reg_DMSIM_INT_array_burst_write( b_ptr,
                                              h_ptr,
                                              0,
                                              3,
                                              value);
}

static INLINE void odtu_dmx_lfield_DMSIM_I_get( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_DMSIM_I_get( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_DMSIM_I_get");
    odtu_dmx_reg_DMSIM_INT_array_burst_read( b_ptr,
                                             h_ptr,
                                             0,
                                             3,
                                             (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_DMSIM_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_DMSIM_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_DMSIM_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_DMSIM_INT_array_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            word_number,
                                                            mask,
                                                            field_ofs,
                                                            value);
}

static INLINE UINT32 odtu_dmx_lfield_range_DMSIM_I_get( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_DMSIM_I_get( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_DMSIM_INT_array_read( b_ptr,
                                                   h_ptr,
                                                   word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_DMSIM_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DMSIM_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DMSIM_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_DMSIM_INT_array_poll( b_ptr,
                                              h_ptr,
                                              word_number,
                                              mask,
                                              ( value << field_ofs),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_STS_SW_OVFLW_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_STS_SW_OVFLW_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_STS_SW_OVFLW_I_set_to_clear");
    odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_write( b_ptr,
                                                     h_ptr,
                                                     0,
                                                     3,
                                                     value);
}

static INLINE void odtu_dmx_lfield_STS_SW_OVFLW_I_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_STS_SW_OVFLW_I_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_STS_SW_OVFLW_I_get");
    odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_read( b_ptr,
                                                    h_ptr,
                                                    0,
                                                    3,
                                                    (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_STS_SW_OVFLW_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_STS_SW_OVFLW_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_STS_SW_OVFLW_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_STS_SW_OVFLW_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_STS_SW_OVFLW_INT_array_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   word_number,
                                                                   mask,
                                                                   field_ofs,
                                                                   value);
}

static INLINE UINT32 odtu_dmx_lfield_range_STS_SW_OVFLW_I_get( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_STS_SW_OVFLW_I_get( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_STS_SW_OVFLW_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_STS_SW_OVFLW_INT_array_read( b_ptr,
                                                          h_ptr,
                                                          word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_STS_SW_OVFLW_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_STS_SW_OVFLW_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                              odtu_dmx_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_STS_SW_OVFLW_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                              odtu_dmx_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_STS_SW_OVFLW_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_STS_SW_OVFLW_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_STS_SW_OVFLW_INT_array_poll( b_ptr,
                                                     h_ptr,
                                                     word_number,
                                                     mask,
                                                     ( value << field_ofs),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_SSF_SF_RCVD_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_SSF_SF_RCVD_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_SSF_SF_RCVD_I_set_to_clear");
    odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_write( b_ptr,
                                                    h_ptr,
                                                    0,
                                                    3,
                                                    value);
}

static INLINE void odtu_dmx_lfield_SSF_SF_RCVD_I_get( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_SSF_SF_RCVD_I_get( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_SSF_SF_RCVD_I_get");
    odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_read( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   3,
                                                   (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_SSF_SF_RCVD_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_SSF_SF_RCVD_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_SSF_SF_RCVD_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_SSF_SF_RCVD_INT_array_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  word_number,
                                                                  mask,
                                                                  field_ofs,
                                                                  value);
}

static INLINE UINT32 odtu_dmx_lfield_range_SSF_SF_RCVD_I_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_SSF_SF_RCVD_I_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_SSF_SF_RCVD_INT_array_read( b_ptr,
                                                         h_ptr,
                                                         word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_SSF_SF_RCVD_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_SSF_SF_RCVD_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_SSF_SF_RCVD_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_SSF_SF_RCVD_INT_array_poll( b_ptr,
                                                    h_ptr,
                                                    word_number,
                                                    mask,
                                                    ( value << field_ofs),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_CM_OUT_LIMIT_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CM_OUT_LIMIT_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                odtu_dmx_handle_t *h_ptr,
                                                                UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CM_OUT_LIMIT_I_set_to_clear");
    odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_write( b_ptr,
                                                        h_ptr,
                                                        0,
                                                        3,
                                                        value);
}

static INLINE void odtu_dmx_lfield_CM_OUT_LIMIT_I_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CM_OUT_LIMIT_I_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CM_OUT_LIMIT_I_get");
    odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_read( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_CM_OUT_LIMIT_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_CM_OUT_LIMIT_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_OUT_LIMIT_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CM_OUT_LIMIT_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_action_on_write_field_set( b_ptr,
                                                                      h_ptr,
                                                                      word_number,
                                                                      mask,
                                                                      field_ofs,
                                                                      value);
}

static INLINE UINT32 odtu_dmx_lfield_range_CM_OUT_LIMIT_I_get( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_CM_OUT_LIMIT_I_get( odtu_dmx_buffer_t *b_ptr,
                                                               odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_OUT_LIMIT_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_read( b_ptr,
                                                             h_ptr,
                                                             word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CM_OUT_LIMIT_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CM_OUT_LIMIT_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                              odtu_dmx_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CM_OUT_LIMIT_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                              odtu_dmx_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_OUT_LIMIT_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_CM_OUT_LIMIT_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_poll( b_ptr,
                                                        h_ptr,
                                                        word_number,
                                                        mask,
                                                        ( value << field_ofs),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_CND_MAX_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CND_MAX_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CND_MAX_I_set_to_clear");
    odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_write( b_ptr,
                                                              h_ptr,
                                                              0,
                                                              3,
                                                              value);
}

static INLINE void odtu_dmx_lfield_CND_MAX_I_get( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CND_MAX_I_get( odtu_dmx_buffer_t *b_ptr,
                                                  odtu_dmx_handle_t *h_ptr,
                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CND_MAX_I_get");
    odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_read( b_ptr,
                                                             h_ptr,
                                                             0,
                                                             3,
                                                             (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_CND_MAX_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_CND_MAX_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CND_MAX_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CND_MAX_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_action_on_write_field_set( b_ptr,
                                                                            h_ptr,
                                                                            word_number,
                                                                            mask,
                                                                            field_ofs,
                                                                            value);
}

static INLINE UINT32 odtu_dmx_lfield_range_CND_MAX_I_get( odtu_dmx_buffer_t *b_ptr,
                                                          odtu_dmx_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_CND_MAX_I_get( odtu_dmx_buffer_t *b_ptr,
                                                          odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CND_MAX_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_read( b_ptr,
                                                                   h_ptr,
                                                                   word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CND_MAX_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CND_MAX_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                         odtu_dmx_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CND_MAX_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                         odtu_dmx_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CND_MAX_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_CND_MAX_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_poll( b_ptr,
                                                              h_ptr,
                                                              word_number,
                                                              mask,
                                                              ( value << field_ofs),
                                                              cmp,
                                                              max_count,
                                                              num_failed_polls,
                                                              delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                     odtu_dmx_handle_t *h_ptr,
                                                                     UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_I_set_to_clear");
    odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_write( b_ptr,
                                                           h_ptr,
                                                           0,
                                                           3,
                                                           value);
}

static INLINE void odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_I_get( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_I_get( odtu_dmx_buffer_t *b_ptr,
                                                            odtu_dmx_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_I_get");
    odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_read( b_ptr,
                                                          h_ptr,
                                                          0,
                                                          3,
                                                          (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                           odtu_dmx_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                           odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_action_on_write_field_set( b_ptr,
                                                                         h_ptr,
                                                                         word_number,
                                                                         mask,
                                                                         field_ofs,
                                                                         value);
}

static INLINE UINT32 odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_read( b_ptr,
                                                                h_ptr,
                                                                word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                   odtu_dmx_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                   odtu_dmx_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_poll( b_ptr,
                                                           h_ptr,
                                                           word_number,
                                                           mask,
                                                           ( value << field_ofs),
                                                           cmp,
                                                           max_count,
                                                           num_failed_polls,
                                                           delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_CM_CND_CRC_ERR_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CM_CND_CRC_ERR_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CM_CND_CRC_ERR_I_set_to_clear");
    odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void odtu_dmx_lfield_CM_CND_CRC_ERR_I_get( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_CM_CND_CRC_ERR_I_get( odtu_dmx_buffer_t *b_ptr,
                                                         odtu_dmx_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_CM_CND_CRC_ERR_I_get");
    odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                        odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     word_number,
                                                                     mask,
                                                                     field_ofs,
                                                                     value);
}

static INLINE UINT32 odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                odtu_dmx_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                odtu_dmx_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_poll( b_ptr,
                                                       h_ptr,
                                                       word_number,
                                                       mask,
                                                       ( value << field_ofs),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_ERLY_PSERV_EXPRY_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_ERLY_PSERV_EXPRY_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_ERLY_PSERV_EXPRY_I_set_to_clear");
    odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_write( b_ptr,
                                                         h_ptr,
                                                         0,
                                                         3,
                                                         value);
}

static INLINE void odtu_dmx_lfield_ERLY_PSERV_EXPRY_I_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_ERLY_PSERV_EXPRY_I_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_ERLY_PSERV_EXPRY_I_get");
    odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_read( b_ptr,
                                                        h_ptr,
                                                        0,
                                                        3,
                                                        (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       word_number,
                                                                       mask,
                                                                       field_ofs,
                                                                       value);
}

static INLINE UINT32 odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_read( b_ptr,
                                                              h_ptr,
                                                              word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                  odtu_dmx_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                  odtu_dmx_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_poll( b_ptr,
                                                         h_ptr,
                                                         word_number,
                                                         mask,
                                                         ( value << field_ofs),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_LATE_PSERV_EXPRY_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_LATE_PSERV_EXPRY_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                    odtu_dmx_handle_t *h_ptr,
                                                                    UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_LATE_PSERV_EXPRY_I_set_to_clear");
    odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_write( b_ptr,
                                                         h_ptr,
                                                         0,
                                                         3,
                                                         value);
}

static INLINE void odtu_dmx_lfield_LATE_PSERV_EXPRY_I_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_LATE_PSERV_EXPRY_I_get( odtu_dmx_buffer_t *b_ptr,
                                                           odtu_dmx_handle_t *h_ptr,
                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_LATE_PSERV_EXPRY_I_get");
    odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_read( b_ptr,
                                                        h_ptr,
                                                        0,
                                                        3,
                                                        (UINT32 *)value);
}

static INLINE void odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_set_to_clear( odtu_dmx_buffer_t *b_ptr,
                                                                          odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_set_to_clear", start_bit, stop_bit, value);
    odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       word_number,
                                                                       mask,
                                                                       field_ofs,
                                                                       value);
}

static INLINE UINT32 odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_get( odtu_dmx_buffer_t *b_ptr,
                                                                   odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_read( b_ptr,
                                                              h_ptr,
                                                              word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                  odtu_dmx_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_poll( odtu_dmx_buffer_t *b_ptr,
                                                                                  odtu_dmx_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_poll( b_ptr,
                                                         h_ptr,
                                                         word_number,
                                                         mask,
                                                         ( value << field_ofs),
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
static INLINE UINT32 odtu_dmx_field_DLOOMFI_V_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_DLOOMFI_V_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000a68 bits 0) field DLOOMFI_V of register PMC_ODTU_DMX_REG_DLOOMFI_INT_VAL */
    reg_value = odtu_dmx_reg_DLOOMFI_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ODTU_DMX_REG_DLOOMFI_INT_VAL_BIT_DLOOMFI_V_MSK) >> ODTU_DMX_REG_DLOOMFI_INT_VAL_BIT_DLOOMFI_V_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_DLOOMFI_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_DLOOMFI_V_poll( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_DLOOMFI_V_poll( odtu_dmx_buffer_t *b_ptr,
                                                                  odtu_dmx_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odtu_dmx_field_DLOOMFI_V_poll", value );

    /* (0x00000a68 bits 0) field DLOOMFI_V of register PMC_ODTU_DMX_REG_DLOOMFI_INT_VAL */
    return odtu_dmx_reg_DLOOMFI_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              ODTU_DMX_REG_DLOOMFI_INT_VAL_BIT_DLOOMFI_V_MSK,
                                              (value<<ODTU_DMX_REG_DLOOMFI_INT_VAL_BIT_DLOOMFI_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 odtu_dmx_field_OOM_IM_V_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_OOM_IM_V_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000a6c bits 0) field OOM_IM_V of register PMC_ODTU_DMX_REG_OOM_IM_INT_VAL */
    reg_value = odtu_dmx_reg_OOM_IM_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ODTU_DMX_REG_OOM_IM_INT_VAL_BIT_OOM_IM_V_MSK) >> ODTU_DMX_REG_OOM_IM_INT_VAL_BIT_OOM_IM_V_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_OOM_IM_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_OOM_IM_V_poll( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_field_OOM_IM_V_poll( odtu_dmx_buffer_t *b_ptr,
                                                                 odtu_dmx_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odtu_dmx_field_OOM_IM_V_poll", value );

    /* (0x00000a6c bits 0) field OOM_IM_V of register PMC_ODTU_DMX_REG_OOM_IM_INT_VAL */
    return odtu_dmx_reg_OOM_IM_INT_VAL_poll( b_ptr,
                                             h_ptr,
                                             ODTU_DMX_REG_OOM_IM_INT_VAL_BIT_OOM_IM_V_MSK,
                                             (value<<ODTU_DMX_REG_OOM_IM_INT_VAL_BIT_OOM_IM_V_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value_N_burstsize3
 * ==================================================================================
 */
static INLINE void odtu_dmx_lfield_DPLM_V_get( odtu_dmx_buffer_t *b_ptr,
                                               odtu_dmx_handle_t *h_ptr,
                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_DPLM_V_get( odtu_dmx_buffer_t *b_ptr,
                                               odtu_dmx_handle_t *h_ptr,
                                               UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_DPLM_V_get");
    odtu_dmx_reg_DPLM_INT_VAL_array_burst_read( b_ptr,
                                                h_ptr,
                                                0,
                                                3,
                                                (UINT32 *)value);
}

static INLINE UINT32 odtu_dmx_lfield_range_DPLM_V_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_DPLM_V_get( odtu_dmx_buffer_t *b_ptr,
                                                       odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DPLM_V_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_DPLM_INT_VAL_array_read( b_ptr,
                                                      h_ptr,
                                                      word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_DPLM_V_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DPLM_V_poll( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DPLM_V_poll( odtu_dmx_buffer_t *b_ptr,
                                                                      odtu_dmx_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DPLM_V_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_DPLM_V_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_DPLM_INT_VAL_array_poll( b_ptr,
                                                 h_ptr,
                                                 word_number,
                                                 mask,
                                                 ( value << field_ofs),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_DMSIM_V_get( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_DMSIM_V_get( odtu_dmx_buffer_t *b_ptr,
                                                odtu_dmx_handle_t *h_ptr,
                                                UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_DMSIM_V_get");
    odtu_dmx_reg_DMSIM_INT_VAL_array_burst_read( b_ptr,
                                                 h_ptr,
                                                 0,
                                                 3,
                                                 (UINT32 *)value);
}

static INLINE UINT32 odtu_dmx_lfield_range_DMSIM_V_get( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_DMSIM_V_get( odtu_dmx_buffer_t *b_ptr,
                                                        odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_V_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_DMSIM_INT_VAL_array_read( b_ptr,
                                                       h_ptr,
                                                       word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_DMSIM_V_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DMSIM_V_poll( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DMSIM_V_poll( odtu_dmx_buffer_t *b_ptr,
                                                                       odtu_dmx_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_V_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_V_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_DMSIM_INT_VAL_array_poll( b_ptr,
                                                  h_ptr,
                                                  word_number,
                                                  mask,
                                                  ( value << field_ofs),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void odtu_dmx_lfield_SSF_SF_RCVD_V_get( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odtu_dmx_lfield_SSF_SF_RCVD_V_get( odtu_dmx_buffer_t *b_ptr,
                                                      odtu_dmx_handle_t *h_ptr,
                                                      UINT32 value[3] )
{
    IOLOG( "%s ", "odtu_dmx_lfield_SSF_SF_RCVD_V_get");
    odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_burst_read( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       (UINT32 *)value);
}

static INLINE UINT32 odtu_dmx_lfield_range_SSF_SF_RCVD_V_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_lfield_range_SSF_SF_RCVD_V_get( odtu_dmx_buffer_t *b_ptr,
                                                              odtu_dmx_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_V_get", start_bit, stop_bit );
    }
    reg_value = odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_read( b_ptr,
                                                             h_ptr,
                                                             word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odtu_dmx_lfield_range_SSF_SF_RCVD_V_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_SSF_SF_RCVD_V_poll( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_SSF_SF_RCVD_V_poll( odtu_dmx_buffer_t *b_ptr,
                                                                             odtu_dmx_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_V_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_V_set_to_clear", start_bit, stop_bit);
    return odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_poll( b_ptr,
                                                        h_ptr,
                                                        word_number,
                                                        mask,
                                                        ( value << field_ofs),
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
static INLINE UINT32 odtu_dmx_field_LO_CONFIG_ADDR_ST_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_LO_CONFIG_ADDR_ST_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000a8c bits 8) field LO_CONFIG_ADDR_ST of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_REG */
    reg_value = odtu_dmx_reg_CONFIG_ADDR_ST_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ODTU_DMX_REG_CONFIG_ADDR_ST_REG_BIT_LO_CONFIG_ADDR_ST_MSK) >> ODTU_DMX_REG_CONFIG_ADDR_ST_REG_BIT_LO_CONFIG_ADDR_ST_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_LO_CONFIG_ADDR_ST_get", value );

    return value;
}
static INLINE UINT32 odtu_dmx_field_HO_CONFIG_ADDR_ST_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odtu_dmx_field_HO_CONFIG_ADDR_ST_get( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000a8c bits 0) field HO_CONFIG_ADDR_ST of register PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_REG */
    reg_value = odtu_dmx_reg_CONFIG_ADDR_ST_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ODTU_DMX_REG_CONFIG_ADDR_ST_REG_BIT_HO_CONFIG_ADDR_ST_MSK) >> ODTU_DMX_REG_CONFIG_ADDR_ST_REG_BIT_HO_CONFIG_ADDR_ST_OFF;
    IOLOG( "%s -> 0x%08x", "odtu_dmx_field_HO_CONFIG_ADDR_ST_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ODTU_DMX_IO_INLINE_H */
