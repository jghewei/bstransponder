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
 *     and register accessor functions for the digi_m1 block
 *****************************************************************************/
#ifndef _DIGI_M1_IO_INLINE_H
#define _DIGI_M1_IO_INLINE_H

#include "digi_api.h"
#include "digi_m1_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DIGI_M1_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for digi_m1
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
    digi_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} digi_m1_buffer_t;
static INLINE void digi_m1_buffer_init( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void digi_m1_buffer_init( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "digi_m1_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void digi_m1_buffer_flush( digi_m1_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void digi_m1_buffer_flush( digi_m1_buffer_t *b_ptr )
{
    IOLOG( "digi_m1_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 digi_m1_reg_read( digi_m1_buffer_t *b_ptr,
                                       digi_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_read( digi_m1_buffer_t *b_ptr,
                                       digi_handle_t *h_ptr,
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
static INLINE void digi_m1_reg_write( digi_m1_buffer_t *b_ptr,
                                      digi_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_write( digi_m1_buffer_t *b_ptr,
                                      digi_handle_t *h_ptr,
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

static INLINE void digi_m1_field_set( digi_m1_buffer_t *b_ptr,
                                      digi_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_set( digi_m1_buffer_t *b_ptr,
                                      digi_handle_t *h_ptr,
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

static INLINE void digi_m1_action_on_write_field_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_action_on_write_field_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
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

static INLINE void digi_m1_burst_read( digi_m1_buffer_t *b_ptr,
                                       digi_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void digi_m1_burst_read( digi_m1_buffer_t *b_ptr,
                                       digi_handle_t *h_ptr,
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

static INLINE void digi_m1_burst_write( digi_m1_buffer_t *b_ptr,
                                        digi_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void digi_m1_burst_write( digi_m1_buffer_t *b_ptr,
                                        digi_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE digi_m1_poll( digi_m1_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE digi_m1_poll( digi_m1_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
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
 *  register access functions for digi_m1
 * ==================================================================================
 */

static INLINE void digi_m1_reg_SUBSYSTEM_SW_RESETS_write( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_SUBSYSTEM_SW_RESETS_write( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_SUBSYSTEM_SW_RESETS_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS,
                       value);
}

static INLINE void digi_m1_reg_SUBSYSTEM_SW_RESETS_field_set( digi_m1_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_SUBSYSTEM_SW_RESETS_field_set( digi_m1_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_SUBSYSTEM_SW_RESETS_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS,
                       mask,
                       PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_SUBSYSTEM_SW_RESETS_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_SUBSYSTEM_SW_RESETS_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_SUBSYSTEM_SW_RESETS_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_PLL_SUPPORT_MUX_CFG_write( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_PLL_SUPPORT_MUX_CFG_write( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_PLL_SUPPORT_MUX_CFG_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG,
                       value);
}

static INLINE void digi_m1_reg_PLL_SUPPORT_MUX_CFG_field_set( digi_m1_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_PLL_SUPPORT_MUX_CFG_field_set( digi_m1_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_PLL_SUPPORT_MUX_CFG_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG,
                       mask,
                       PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_PLL_SUPPORT_MUX_CFG_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_PLL_SUPPORT_MUX_CFG_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_PLL_SUPPORT_MUX_CFG_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_write( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_write( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0,
                       value);
}

static INLINE void digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_field_set( digi_m1_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_field_set( digi_m1_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0,
                       mask,
                       PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_write( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_write( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1,
                       value);
}

static INLINE void digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_field_set( digi_m1_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_field_set( digi_m1_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1,
                       mask,
                       PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_IRIG_IF_MUX_write( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_IRIG_IF_MUX_write( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_IRIG_IF_MUX_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_IRIG_IF_MUX,
                       value);
}

static INLINE void digi_m1_reg_IRIG_IF_MUX_field_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_IRIG_IF_MUX_field_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_IRIG_IF_MUX_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_IRIG_IF_MUX,
                       mask,
                       PMC_DIGI_M1_REG_IRIG_IF_MUX_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_IRIG_IF_MUX_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_IRIG_IF_MUX_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_IRIG_IF_MUX);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_IRIG_IF_MUX_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_CAL_349_CFG_0_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_349_CFG_0_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_CAL_349_CFG_0_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_349_CFG_0,
                       value);
}

static INLINE void digi_m1_reg_CAL_349_CFG_0_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_349_CFG_0_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_CAL_349_CFG_0_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_349_CFG_0,
                       mask,
                       PMC_DIGI_M1_REG_CAL_349_CFG_0_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_CAL_349_CFG_0_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_CAL_349_CFG_0_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_CAL_349_CFG_0);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_CAL_349_CFG_0_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_CAL_349_CFG_1_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_349_CFG_1_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_CAL_349_CFG_1_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_349_CFG_1,
                       value);
}

static INLINE void digi_m1_reg_CAL_349_CFG_1_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_349_CFG_1_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_CAL_349_CFG_1_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_349_CFG_1,
                       mask,
                       PMC_DIGI_M1_REG_CAL_349_CFG_1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_CAL_349_CFG_1_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_CAL_349_CFG_1_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_CAL_349_CFG_1);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_CAL_349_CFG_1_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_CAL_349_CFG_2_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_349_CFG_2_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_CAL_349_CFG_2_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_349_CFG_2,
                       value);
}

static INLINE void digi_m1_reg_CAL_349_CFG_2_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_349_CFG_2_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_CAL_349_CFG_2_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_349_CFG_2,
                       mask,
                       PMC_DIGI_M1_REG_CAL_349_CFG_2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_CAL_349_CFG_2_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_CAL_349_CFG_2_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_CAL_349_CFG_2);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_CAL_349_CFG_2_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_CAL_500_CFG_0_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_500_CFG_0_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_CAL_500_CFG_0_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_500_CFG_0,
                       value);
}

static INLINE void digi_m1_reg_CAL_500_CFG_0_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_500_CFG_0_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_CAL_500_CFG_0_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_500_CFG_0,
                       mask,
                       PMC_DIGI_M1_REG_CAL_500_CFG_0_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_CAL_500_CFG_0_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_CAL_500_CFG_0_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_CAL_500_CFG_0);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_CAL_500_CFG_0_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_CAL_500_CFG_1_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_500_CFG_1_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_CAL_500_CFG_1_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_500_CFG_1,
                       value);
}

static INLINE void digi_m1_reg_CAL_500_CFG_1_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_500_CFG_1_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_CAL_500_CFG_1_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_500_CFG_1,
                       mask,
                       PMC_DIGI_M1_REG_CAL_500_CFG_1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_CAL_500_CFG_1_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_CAL_500_CFG_1_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_CAL_500_CFG_1);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_CAL_500_CFG_1_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_CAL_500_CFG_2_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_500_CFG_2_write( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_CAL_500_CFG_2_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_500_CFG_2,
                       value);
}

static INLINE void digi_m1_reg_CAL_500_CFG_2_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_500_CFG_2_field_set( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_CAL_500_CFG_2_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_CAL_500_CFG_2,
                       mask,
                       PMC_DIGI_M1_REG_CAL_500_CFG_2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_CAL_500_CFG_2_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_CAL_500_CFG_2_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_CAL_500_CFG_2);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_CAL_500_CFG_2_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_IRIG_IF_OUTPUT_ENS_write( digi_m1_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_IRIG_IF_OUTPUT_ENS_write( digi_m1_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_IRIG_IF_OUTPUT_ENS_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS,
                       value);
}

static INLINE void digi_m1_reg_IRIG_IF_OUTPUT_ENS_field_set( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_IRIG_IF_OUTPUT_ENS_field_set( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_IRIG_IF_OUTPUT_ENS_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS,
                       mask,
                       PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_IRIG_IF_OUTPUT_ENS_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_IRIG_IF_OUTPUT_ENS_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_IRIG_IF_OUTPUT_ENS_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_OTU_MFPO_SELECT_write( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_OTU_MFPO_SELECT_write( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_OTU_MFPO_SELECT_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_OTU_MFPO_SELECT,
                       value);
}

static INLINE void digi_m1_reg_OTU_MFPO_SELECT_field_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_OTU_MFPO_SELECT_field_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_OTU_MFPO_SELECT_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_DIGI_M1_REG_OTU_MFPO_SELECT,
                       mask,
                       PMC_DIGI_M1_REG_OTU_MFPO_SELECT_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_OTU_MFPO_SELECT_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_OTU_MFPO_SELECT_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_DIGI_M1_REG_OTU_MFPO_SELECT);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_OTU_MFPO_SELECT_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_CAL_349_REQ_write( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_349_REQ_write( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_CAL_349_REQ_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_DIGI_M1_REG_CAL_349_REQ,
                       value);
}

static INLINE void digi_m1_reg_CAL_349_REQ_field_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_349_REQ_field_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_CAL_349_REQ_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_DIGI_M1_REG_CAL_349_REQ,
                       mask,
                       PMC_DIGI_M1_REG_CAL_349_REQ_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_CAL_349_REQ_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_CAL_349_REQ_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_DIGI_M1_REG_CAL_349_REQ);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_CAL_349_REQ_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_CAL_500_REQ_write( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_500_REQ_write( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "digi_m1_reg_CAL_500_REQ_write", value );
    digi_m1_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_DIGI_M1_REG_CAL_500_REQ,
                       value);
}

static INLINE void digi_m1_reg_CAL_500_REQ_field_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_CAL_500_REQ_field_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "digi_m1_reg_CAL_500_REQ_field_set", mask, ofs, value );
    digi_m1_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_DIGI_M1_REG_CAL_500_REQ,
                       mask,
                       PMC_DIGI_M1_REG_CAL_500_REQ_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 digi_m1_reg_CAL_500_REQ_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_CAL_500_REQ_read( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_DIGI_M1_REG_CAL_500_REQ);

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_CAL_500_REQ_read", reg_value);
    return reg_value;
}

static INLINE void digi_m1_reg_EFUSE_STAT_array_burst_read( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 ofs,
                                                            UINT32 len,
                                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void digi_m1_reg_EFUSE_STAT_array_burst_read( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 ofs,
                                                            UINT32 len,
                                                            UINT32 *value )
{
    if (len + ofs > 10)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "digi_m1_reg_EFUSE_STAT_array_burst_read", ofs, len, 10 );
    digi_m1_burst_read( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_DIGI_M1_REG_EFUSE_STAT(ofs),
                        len,
                        value);

    IOLOG( "%s of %d words ofs=%d len=%d", "digi_m1_reg_EFUSE_STAT_array_burst_read", 10, ofs, len);
}

static INLINE UINT32 digi_m1_reg_EFUSE_STAT_array_read( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_reg_EFUSE_STAT_array_read( digi_m1_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 N )
{
    UINT32 reg_value;

    reg_value = digi_m1_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_DIGI_M1_REG_EFUSE_STAT(N));

    IOLOG( "%s -> 0x%08x;", "digi_m1_reg_EFUSE_STAT_array_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void digi_m1_field_FEGE_SW_RST_set( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_FEGE_SW_RST_set( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_FEGE_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_FEGE_SW_RST_set", value );

    /* (0x00016000 bits 7) field FEGE_SW_RST of register PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS */
    digi_m1_reg_SUBSYSTEM_SW_RESETS_field_set( b_ptr,
                                               h_ptr,
                                               DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_FEGE_SW_RST_MSK,
                                               DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_FEGE_SW_RST_OFF,
                                               value);
}

static INLINE UINT32 digi_m1_field_FEGE_SW_RST_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_FEGE_SW_RST_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016000 bits 7) field FEGE_SW_RST of register PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS */
    reg_value = digi_m1_reg_SUBSYSTEM_SW_RESETS_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_FEGE_SW_RST_MSK) >> DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_FEGE_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_FEGE_SW_RST_get", value );

    return value;
}
static INLINE void digi_m1_field_PGMRCLK_SW_RST_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_PGMRCLK_SW_RST_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_PGMRCLK_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_PGMRCLK_SW_RST_set", value );

    /* (0x00016000 bits 6) field PGMRCLK_SW_RST of register PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS */
    digi_m1_reg_SUBSYSTEM_SW_RESETS_field_set( b_ptr,
                                               h_ptr,
                                               DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_PGMRCLK_SW_RST_MSK,
                                               DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_PGMRCLK_SW_RST_OFF,
                                               value);
}

static INLINE UINT32 digi_m1_field_PGMRCLK_SW_RST_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_PGMRCLK_SW_RST_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016000 bits 6) field PGMRCLK_SW_RST of register PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS */
    reg_value = digi_m1_reg_SUBSYSTEM_SW_RESETS_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_PGMRCLK_SW_RST_MSK) >> DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_PGMRCLK_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_PGMRCLK_SW_RST_get", value );

    return value;
}
static INLINE void digi_m1_field_TOC_CAL_349_SW_RST_set( digi_m1_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_TOC_CAL_349_SW_RST_set( digi_m1_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_TOC_CAL_349_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_TOC_CAL_349_SW_RST_set", value );

    /* (0x00016000 bits 5) field TOC_CAL_349_SW_RST of register PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS */
    digi_m1_reg_SUBSYSTEM_SW_RESETS_field_set( b_ptr,
                                               h_ptr,
                                               DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_349_SW_RST_MSK,
                                               DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_349_SW_RST_OFF,
                                               value);
}

static INLINE UINT32 digi_m1_field_TOC_CAL_349_SW_RST_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_TOC_CAL_349_SW_RST_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016000 bits 5) field TOC_CAL_349_SW_RST of register PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS */
    reg_value = digi_m1_reg_SUBSYSTEM_SW_RESETS_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_349_SW_RST_MSK) >> DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_349_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_TOC_CAL_349_SW_RST_get", value );

    return value;
}
static INLINE void digi_m1_field_TOC_CAL_500_SW_RST_set( digi_m1_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_TOC_CAL_500_SW_RST_set( digi_m1_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_TOC_CAL_500_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_TOC_CAL_500_SW_RST_set", value );

    /* (0x00016000 bits 4) field TOC_CAL_500_SW_RST of register PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS */
    digi_m1_reg_SUBSYSTEM_SW_RESETS_field_set( b_ptr,
                                               h_ptr,
                                               DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_500_SW_RST_MSK,
                                               DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_500_SW_RST_OFF,
                                               value);
}

static INLINE UINT32 digi_m1_field_TOC_CAL_500_SW_RST_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_TOC_CAL_500_SW_RST_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016000 bits 4) field TOC_CAL_500_SW_RST of register PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS */
    reg_value = digi_m1_reg_SUBSYSTEM_SW_RESETS_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_500_SW_RST_MSK) >> DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_500_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_TOC_CAL_500_SW_RST_get", value );

    return value;
}
static INLINE void digi_m1_field_MISC_SW_RST_set( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_MISC_SW_RST_set( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_MISC_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_MISC_SW_RST_set", value );

    /* (0x00016000 bits 3) field MISC_SW_RST of register PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS */
    digi_m1_reg_SUBSYSTEM_SW_RESETS_field_set( b_ptr,
                                               h_ptr,
                                               DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_MISC_SW_RST_MSK,
                                               DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_MISC_SW_RST_OFF,
                                               value);
}

static INLINE UINT32 digi_m1_field_MISC_SW_RST_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_MISC_SW_RST_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016000 bits 3) field MISC_SW_RST of register PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS */
    reg_value = digi_m1_reg_SUBSYSTEM_SW_RESETS_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_MISC_SW_RST_MSK) >> DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_MISC_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_MISC_SW_RST_get", value );

    return value;
}
static INLINE void digi_m1_field_LINE_TIMING_REF_SEL_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_LINE_TIMING_REF_SEL_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_LINE_TIMING_REF_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_LINE_TIMING_REF_SEL_set", value );

    /* (0x00016004 bits 8) field LINE_TIMING_REF_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
    digi_m1_reg_PLL_SUPPORT_MUX_CFG_field_set( b_ptr,
                                               h_ptr,
                                               DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_TIMING_REF_SEL_MSK,
                                               DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_TIMING_REF_SEL_OFF,
                                               value);
}

static INLINE UINT32 digi_m1_field_LINE_TIMING_REF_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_LINE_TIMING_REF_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016004 bits 8) field LINE_TIMING_REF_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
    reg_value = digi_m1_reg_PLL_SUPPORT_MUX_CFG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_TIMING_REF_SEL_MSK) >> DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_TIMING_REF_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_LINE_TIMING_REF_SEL_get", value );

    return value;
}
static INLINE void digi_m1_field_LINE_REF_CLK_SEL_set( digi_m1_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_LINE_REF_CLK_SEL_set( digi_m1_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_LINE_REF_CLK_SEL_set", value, 7);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_LINE_REF_CLK_SEL_set", value );

    /* (0x00016004 bits 6:4) bits 0:2 use field LINE_REF_CLK_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
    digi_m1_reg_PLL_SUPPORT_MUX_CFG_field_set( b_ptr,
                                               h_ptr,
                                               DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_MSK,
                                               DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_OFF,
                                               value);
}

static INLINE UINT32 digi_m1_field_LINE_REF_CLK_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_LINE_REF_CLK_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016004 bits 6:4) bits 0:2 use field LINE_REF_CLK_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
    reg_value = digi_m1_reg_PLL_SUPPORT_MUX_CFG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_MSK) >> DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_LINE_REF_CLK_SEL_get", value );

    return value;
}
static INLINE void digi_m1_field_range_LINE_REF_CLK_SEL_set( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_LINE_REF_CLK_SEL_set( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_LINE_REF_CLK_SEL_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_LINE_REF_CLK_SEL_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_LINE_REF_CLK_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016004 bits 6:4) bits 0:2 use field LINE_REF_CLK_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
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
        /* (0x00016004 bits 6:4) bits 0:2 use field LINE_REF_CLK_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
        digi_m1_reg_PLL_SUPPORT_MUX_CFG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_OFF + subfield_offset),
                                                   DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_LINE_REF_CLK_SEL_get( digi_m1_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_LINE_REF_CLK_SEL_get( digi_m1_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_LINE_REF_CLK_SEL_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_LINE_REF_CLK_SEL_get", stop_bit, 2 );
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
    /* (0x00016004 bits 6:4) bits 0:2 use field LINE_REF_CLK_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
    reg_value = digi_m1_reg_PLL_SUPPORT_MUX_CFG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_MSK)
                  >> DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_MSK, DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_LINE_REF_CLK_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_PTP_DCSU_REF_SEL_set( digi_m1_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_PTP_DCSU_REF_SEL_set( digi_m1_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_PTP_DCSU_REF_SEL_set", value, 7);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_PTP_DCSU_REF_SEL_set", value );

    /* (0x00016004 bits 2:0) bits 0:2 use field PTP_DCSU_REF_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
    digi_m1_reg_PLL_SUPPORT_MUX_CFG_field_set( b_ptr,
                                               h_ptr,
                                               DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_MSK,
                                               DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_OFF,
                                               value);
}

static INLINE UINT32 digi_m1_field_PTP_DCSU_REF_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_PTP_DCSU_REF_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016004 bits 2:0) bits 0:2 use field PTP_DCSU_REF_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
    reg_value = digi_m1_reg_PLL_SUPPORT_MUX_CFG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_MSK) >> DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_PTP_DCSU_REF_SEL_get", value );

    return value;
}
static INLINE void digi_m1_field_range_PTP_DCSU_REF_SEL_set( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_PTP_DCSU_REF_SEL_set( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_PTP_DCSU_REF_SEL_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_PTP_DCSU_REF_SEL_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_PTP_DCSU_REF_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016004 bits 2:0) bits 0:2 use field PTP_DCSU_REF_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
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
        /* (0x00016004 bits 2:0) bits 0:2 use field PTP_DCSU_REF_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
        digi_m1_reg_PLL_SUPPORT_MUX_CFG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_OFF + subfield_offset),
                                                   DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_PTP_DCSU_REF_SEL_get( digi_m1_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_PTP_DCSU_REF_SEL_get( digi_m1_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_PTP_DCSU_REF_SEL_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_PTP_DCSU_REF_SEL_get", stop_bit, 2 );
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
    /* (0x00016004 bits 2:0) bits 0:2 use field PTP_DCSU_REF_SEL of register PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG */
    reg_value = digi_m1_reg_PLL_SUPPORT_MUX_CFG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_MSK)
                  >> DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_MSK, DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_PTP_DCSU_REF_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_PMM_DSM_DENOM_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_PMM_DSM_DENOM_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 16383)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_PMM_DSM_DENOM_set", value, 16383);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_PMM_DSM_DENOM_set", value );

    /* (0x00016008 bits 29:16) bits 0:13 use field PMM_DSM_DENOM of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0 */
    digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_field_set( b_ptr,
                                                 h_ptr,
                                                 DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_MSK,
                                                 DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_OFF,
                                                 value);
}

static INLINE UINT32 digi_m1_field_PMM_DSM_DENOM_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_PMM_DSM_DENOM_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016008 bits 29:16) bits 0:13 use field PMM_DSM_DENOM of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0 */
    reg_value = digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_MSK) >> DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_PMM_DSM_DENOM_get", value );

    return value;
}
static INLINE void digi_m1_field_range_PMM_DSM_DENOM_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_PMM_DSM_DENOM_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_PMM_DSM_DENOM_set", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_PMM_DSM_DENOM_set", stop_bit, 13 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_PMM_DSM_DENOM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016008 bits 29:16) bits 0:13 use field PMM_DSM_DENOM of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0 */
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
        if (stop_bit < 13) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 13;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00016008 bits 29:16) bits 0:13 use field PMM_DSM_DENOM of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0 */
        digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_OFF + subfield_offset),
                                                     DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_PMM_DSM_DENOM_get( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_PMM_DSM_DENOM_get( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_PMM_DSM_DENOM_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_PMM_DSM_DENOM_get", stop_bit, 13 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 13) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 13;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00016008 bits 29:16) bits 0:13 use field PMM_DSM_DENOM of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0 */
    reg_value = digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_MSK)
                  >> DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_MSK, DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_PMM_DSM_DENOM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_PMM_PKT_PERIOD_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_PMM_PKT_PERIOD_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_PMM_PKT_PERIOD_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_PMM_PKT_PERIOD_set", value );

    /* (0x00016008 bits 15:0) bits 0:15 use field PMM_PKT_PERIOD of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0 */
    digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_field_set( b_ptr,
                                                 h_ptr,
                                                 DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_MSK,
                                                 DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_OFF,
                                                 value);
}

static INLINE UINT32 digi_m1_field_PMM_PKT_PERIOD_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_PMM_PKT_PERIOD_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016008 bits 15:0) bits 0:15 use field PMM_PKT_PERIOD of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0 */
    reg_value = digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_MSK) >> DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_PMM_PKT_PERIOD_get", value );

    return value;
}
static INLINE void digi_m1_field_range_PMM_PKT_PERIOD_set( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_PMM_PKT_PERIOD_set( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_PMM_PKT_PERIOD_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_PMM_PKT_PERIOD_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_PMM_PKT_PERIOD_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016008 bits 15:0) bits 0:15 use field PMM_PKT_PERIOD of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0 */
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
        /* (0x00016008 bits 15:0) bits 0:15 use field PMM_PKT_PERIOD of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0 */
        digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_OFF + subfield_offset),
                                                     DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_PMM_PKT_PERIOD_get( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_PMM_PKT_PERIOD_get( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_PMM_PKT_PERIOD_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_PMM_PKT_PERIOD_get", stop_bit, 15 );
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
    /* (0x00016008 bits 15:0) bits 0:15 use field PMM_PKT_PERIOD of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0 */
    reg_value = digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_MSK)
                  >> DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_MSK, DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_PMM_PKT_PERIOD_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_PMM_PKT_SIZE_set( digi_m1_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_PMM_PKT_SIZE_set( digi_m1_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_PMM_PKT_SIZE_set", value, 511);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_PMM_PKT_SIZE_set", value );

    /* (0x0001600c bits 24:16) bits 0:8 use field PMM_PKT_SIZE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
    digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_MSK,
                                                 DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_OFF,
                                                 value);
}

static INLINE UINT32 digi_m1_field_PMM_PKT_SIZE_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_PMM_PKT_SIZE_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001600c bits 24:16) bits 0:8 use field PMM_PKT_SIZE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
    reg_value = digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_MSK) >> DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_PMM_PKT_SIZE_get", value );

    return value;
}
static INLINE void digi_m1_field_range_PMM_PKT_SIZE_set( digi_m1_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_PMM_PKT_SIZE_set( digi_m1_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_PMM_PKT_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_PMM_PKT_SIZE_set", stop_bit, 8 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_PMM_PKT_SIZE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001600c bits 24:16) bits 0:8 use field PMM_PKT_SIZE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
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
        /* (0x0001600c bits 24:16) bits 0:8 use field PMM_PKT_SIZE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
        digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_OFF + subfield_offset),
                                                     DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_PMM_PKT_SIZE_get( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_PMM_PKT_SIZE_get( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_PMM_PKT_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_PMM_PKT_SIZE_get", stop_bit, 8 );
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
    /* (0x0001600c bits 24:16) bits 0:8 use field PMM_PKT_SIZE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
    reg_value = digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_MSK)
                  >> DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_MSK, DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_PMM_PKT_SIZE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_PMM_DSM_TYPE_set( digi_m1_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_PMM_DSM_TYPE_set( digi_m1_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_PMM_DSM_TYPE_set", value, 3);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_PMM_DSM_TYPE_set", value );

    /* (0x0001600c bits 2:1) bits 0:1 use field PMM_DSM_TYPE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
    digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_MSK,
                                                 DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_OFF,
                                                 value);
}

static INLINE UINT32 digi_m1_field_PMM_DSM_TYPE_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_PMM_DSM_TYPE_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001600c bits 2:1) bits 0:1 use field PMM_DSM_TYPE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
    reg_value = digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_MSK) >> DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_PMM_DSM_TYPE_get", value );

    return value;
}
static INLINE void digi_m1_field_range_PMM_DSM_TYPE_set( digi_m1_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_PMM_DSM_TYPE_set( digi_m1_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_PMM_DSM_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_PMM_DSM_TYPE_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_PMM_DSM_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001600c bits 2:1) bits 0:1 use field PMM_DSM_TYPE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
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
        /* (0x0001600c bits 2:1) bits 0:1 use field PMM_DSM_TYPE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
        digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_OFF + subfield_offset),
                                                     DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_PMM_DSM_TYPE_get( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_PMM_DSM_TYPE_get( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_PMM_DSM_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_PMM_DSM_TYPE_get", stop_bit, 1 );
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
    /* (0x0001600c bits 2:1) bits 0:1 use field PMM_DSM_TYPE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
    reg_value = digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_MSK)
                  >> DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_MSK, DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_PMM_DSM_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_PMM_ENABLE_set( digi_m1_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_PMM_ENABLE_set( digi_m1_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_PMM_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_PMM_ENABLE_set", value );

    /* (0x0001600c bits 0) field PMM_ENABLE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
    digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_ENABLE_MSK,
                                                 DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_ENABLE_OFF,
                                                 value);
}

static INLINE UINT32 digi_m1_field_PMM_ENABLE_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_PMM_ENABLE_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001600c bits 0) field PMM_ENABLE of register PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 */
    reg_value = digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_ENABLE_MSK) >> DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_PMM_ENABLE_get", value );

    return value;
}
static INLINE void digi_m1_field_IRIG_CLKO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_IRIG_CLKO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_IRIG_CLKO_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_IRIG_CLKO_SEL_set", value );

    /* (0x00016010 bits 2) field IRIG_CLKO_SEL of register PMC_DIGI_M1_REG_IRIG_IF_MUX */
    digi_m1_reg_IRIG_IF_MUX_field_set( b_ptr,
                                       h_ptr,
                                       DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_CLKO_SEL_MSK,
                                       DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_CLKO_SEL_OFF,
                                       value);
}

static INLINE UINT32 digi_m1_field_IRIG_CLKO_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_IRIG_CLKO_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016010 bits 2) field IRIG_CLKO_SEL of register PMC_DIGI_M1_REG_IRIG_IF_MUX */
    reg_value = digi_m1_reg_IRIG_IF_MUX_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_CLKO_SEL_MSK) >> DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_CLKO_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_IRIG_CLKO_SEL_get", value );

    return value;
}
static INLINE void digi_m1_field_IRIG_DATO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_IRIG_DATO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_IRIG_DATO_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_IRIG_DATO_SEL_set", value );

    /* (0x00016010 bits 1) field IRIG_DATO_SEL of register PMC_DIGI_M1_REG_IRIG_IF_MUX */
    digi_m1_reg_IRIG_IF_MUX_field_set( b_ptr,
                                       h_ptr,
                                       DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_DATO_SEL_MSK,
                                       DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_DATO_SEL_OFF,
                                       value);
}

static INLINE UINT32 digi_m1_field_IRIG_DATO_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_IRIG_DATO_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016010 bits 1) field IRIG_DATO_SEL of register PMC_DIGI_M1_REG_IRIG_IF_MUX */
    reg_value = digi_m1_reg_IRIG_IF_MUX_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_DATO_SEL_MSK) >> DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_DATO_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_IRIG_DATO_SEL_get", value );

    return value;
}
static INLINE void digi_m1_field_ONE_PPSO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_ONE_PPSO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_ONE_PPSO_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_ONE_PPSO_SEL_set", value );

    /* (0x00016010 bits 0) field ONE_PPSO_SEL of register PMC_DIGI_M1_REG_IRIG_IF_MUX */
    digi_m1_reg_IRIG_IF_MUX_field_set( b_ptr,
                                       h_ptr,
                                       DIGI_M1_REG_IRIG_IF_MUX_BIT_ONE_PPSO_SEL_MSK,
                                       DIGI_M1_REG_IRIG_IF_MUX_BIT_ONE_PPSO_SEL_OFF,
                                       value);
}

static INLINE UINT32 digi_m1_field_ONE_PPSO_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_ONE_PPSO_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016010 bits 0) field ONE_PPSO_SEL of register PMC_DIGI_M1_REG_IRIG_IF_MUX */
    reg_value = digi_m1_reg_IRIG_IF_MUX_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_IRIG_IF_MUX_BIT_ONE_PPSO_SEL_MSK) >> DIGI_M1_REG_IRIG_IF_MUX_BIT_ONE_PPSO_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_ONE_PPSO_SEL_get", value );

    return value;
}
static INLINE void digi_m1_field_CNT_349_EN_set( digi_m1_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_CNT_349_EN_set( digi_m1_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_CNT_349_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_CNT_349_EN_set", value );

    /* (0x00016014 bits 21) field CNT_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
    digi_m1_reg_CAL_349_CFG_0_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_EN_MSK,
                                         DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_EN_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_CNT_349_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_CNT_349_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016014 bits 21) field CNT_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
    reg_value = digi_m1_reg_CAL_349_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_EN_MSK) >> DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_EN_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_CNT_349_EN_get", value );

    return value;
}
static INLINE void digi_m1_field_CNT_349_ROLLOVER_set( digi_m1_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_CNT_349_ROLLOVER_set( digi_m1_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_CNT_349_ROLLOVER_set", value, 127);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_CNT_349_ROLLOVER_set", value );

    /* (0x00016014 bits 20:14) bits 0:6 use field CNT_349_ROLLOVER of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
    digi_m1_reg_CAL_349_CFG_0_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_MSK,
                                         DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_CNT_349_ROLLOVER_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_CNT_349_ROLLOVER_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016014 bits 20:14) bits 0:6 use field CNT_349_ROLLOVER of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
    reg_value = digi_m1_reg_CAL_349_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_MSK) >> DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_CNT_349_ROLLOVER_get", value );

    return value;
}
static INLINE void digi_m1_field_range_CNT_349_ROLLOVER_set( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_CNT_349_ROLLOVER_set( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_CNT_349_ROLLOVER_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_CNT_349_ROLLOVER_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_CNT_349_ROLLOVER_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016014 bits 20:14) bits 0:6 use field CNT_349_ROLLOVER of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
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
        /* (0x00016014 bits 20:14) bits 0:6 use field CNT_349_ROLLOVER of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
        digi_m1_reg_CAL_349_CFG_0_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_OFF + subfield_offset),
                                             DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_CNT_349_ROLLOVER_get( digi_m1_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_CNT_349_ROLLOVER_get( digi_m1_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_CNT_349_ROLLOVER_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_CNT_349_ROLLOVER_get", stop_bit, 6 );
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
    /* (0x00016014 bits 20:14) bits 0:6 use field CNT_349_ROLLOVER of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
    reg_value = digi_m1_reg_CAL_349_CFG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_MSK)
                  >> DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_MSK, DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_CNT_349_ROLLOVER_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_SS_CAL_349_DLY_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_SS_CAL_349_DLY_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_SS_CAL_349_DLY_set", value, 127);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_SS_CAL_349_DLY_set", value );

    /* (0x00016014 bits 13:7) bits 0:6 use field SS_CAL_349_DLY of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
    digi_m1_reg_CAL_349_CFG_0_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_MSK,
                                         DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_SS_CAL_349_DLY_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_SS_CAL_349_DLY_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016014 bits 13:7) bits 0:6 use field SS_CAL_349_DLY of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
    reg_value = digi_m1_reg_CAL_349_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_MSK) >> DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_SS_CAL_349_DLY_get", value );

    return value;
}
static INLINE void digi_m1_field_range_SS_CAL_349_DLY_set( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_SS_CAL_349_DLY_set( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_CAL_349_DLY_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_CAL_349_DLY_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_CAL_349_DLY_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016014 bits 13:7) bits 0:6 use field SS_CAL_349_DLY of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
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
        /* (0x00016014 bits 13:7) bits 0:6 use field SS_CAL_349_DLY of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
        digi_m1_reg_CAL_349_CFG_0_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_OFF + subfield_offset),
                                             DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_SS_CAL_349_DLY_get( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_SS_CAL_349_DLY_get( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_CAL_349_DLY_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_CAL_349_DLY_get", stop_bit, 6 );
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
    /* (0x00016014 bits 13:7) bits 0:6 use field SS_CAL_349_DLY of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
    reg_value = digi_m1_reg_CAL_349_CFG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_MSK)
                  >> DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_MSK, DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_CAL_349_DLY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_SS_TOC_349_DLY_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_SS_TOC_349_DLY_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_SS_TOC_349_DLY_set", value, 127);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_SS_TOC_349_DLY_set", value );

    /* (0x00016014 bits 6:0) bits 0:6 use field SS_TOC_349_DLY of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
    digi_m1_reg_CAL_349_CFG_0_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_MSK,
                                         DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_SS_TOC_349_DLY_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_SS_TOC_349_DLY_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016014 bits 6:0) bits 0:6 use field SS_TOC_349_DLY of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
    reg_value = digi_m1_reg_CAL_349_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_MSK) >> DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_SS_TOC_349_DLY_get", value );

    return value;
}
static INLINE void digi_m1_field_range_SS_TOC_349_DLY_set( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_SS_TOC_349_DLY_set( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_TOC_349_DLY_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_TOC_349_DLY_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_TOC_349_DLY_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016014 bits 6:0) bits 0:6 use field SS_TOC_349_DLY of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
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
        /* (0x00016014 bits 6:0) bits 0:6 use field SS_TOC_349_DLY of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
        digi_m1_reg_CAL_349_CFG_0_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_OFF + subfield_offset),
                                             DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_SS_TOC_349_DLY_get( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_SS_TOC_349_DLY_get( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_TOC_349_DLY_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_TOC_349_DLY_get", stop_bit, 6 );
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
    /* (0x00016014 bits 6:0) bits 0:6 use field SS_TOC_349_DLY of register PMC_DIGI_M1_REG_CAL_349_CFG_0 */
    reg_value = digi_m1_reg_CAL_349_CFG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_MSK)
                  >> DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_MSK, DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_TOC_349_DLY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_SS_TOC_349_EN_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_SS_TOC_349_EN_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_SS_TOC_349_EN_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_SS_TOC_349_EN_set", value );

    /* (0x00016018 bits 9:0) bits 0:9 use field SS_TOC_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_1 */
    digi_m1_reg_CAL_349_CFG_1_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_MSK,
                                         DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_SS_TOC_349_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_SS_TOC_349_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016018 bits 9:0) bits 0:9 use field SS_TOC_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_1 */
    reg_value = digi_m1_reg_CAL_349_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_MSK) >> DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_SS_TOC_349_EN_get", value );

    return value;
}
static INLINE void digi_m1_field_range_SS_TOC_349_EN_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_SS_TOC_349_EN_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_TOC_349_EN_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_TOC_349_EN_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_TOC_349_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016018 bits 9:0) bits 0:9 use field SS_TOC_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_1 */
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
        /* (0x00016018 bits 9:0) bits 0:9 use field SS_TOC_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_1 */
        digi_m1_reg_CAL_349_CFG_1_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_OFF + subfield_offset),
                                             DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_SS_TOC_349_EN_get( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_SS_TOC_349_EN_get( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_TOC_349_EN_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_TOC_349_EN_get", stop_bit, 9 );
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
    /* (0x00016018 bits 9:0) bits 0:9 use field SS_TOC_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_1 */
    reg_value = digi_m1_reg_CAL_349_CFG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_MSK)
                  >> DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_MSK, DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_TOC_349_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_SS_CAL_349_EN_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_SS_CAL_349_EN_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 33554431)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_SS_CAL_349_EN_set", value, 33554431);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_SS_CAL_349_EN_set", value );

    /* (0x0001601c bits 24:0) bits 0:24 use field SS_CAL_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_2 */
    digi_m1_reg_CAL_349_CFG_2_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_MSK,
                                         DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_SS_CAL_349_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_SS_CAL_349_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001601c bits 24:0) bits 0:24 use field SS_CAL_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_2 */
    reg_value = digi_m1_reg_CAL_349_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_MSK) >> DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_SS_CAL_349_EN_get", value );

    return value;
}
static INLINE void digi_m1_field_range_SS_CAL_349_EN_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_SS_CAL_349_EN_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_CAL_349_EN_set", stop_bit, start_bit );
    if (stop_bit > 24) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_CAL_349_EN_set", stop_bit, 24 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_CAL_349_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001601c bits 24:0) bits 0:24 use field SS_CAL_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_2 */
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
        if (stop_bit < 24) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 24;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0001601c bits 24:0) bits 0:24 use field SS_CAL_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_2 */
        digi_m1_reg_CAL_349_CFG_2_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_OFF + subfield_offset),
                                             DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_SS_CAL_349_EN_get( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_SS_CAL_349_EN_get( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_CAL_349_EN_get", stop_bit, start_bit );
    if (stop_bit > 24) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_CAL_349_EN_get", stop_bit, 24 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 24) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 24;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0001601c bits 24:0) bits 0:24 use field SS_CAL_349_EN of register PMC_DIGI_M1_REG_CAL_349_CFG_2 */
    reg_value = digi_m1_reg_CAL_349_CFG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_MSK)
                  >> DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_MSK, DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_CAL_349_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_CNT_500_EN_set( digi_m1_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_CNT_500_EN_set( digi_m1_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_CNT_500_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_CNT_500_EN_set", value );

    /* (0x0001603c bits 21) field CNT_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
    digi_m1_reg_CAL_500_CFG_0_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_EN_MSK,
                                         DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_EN_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_CNT_500_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_CNT_500_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001603c bits 21) field CNT_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
    reg_value = digi_m1_reg_CAL_500_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_EN_MSK) >> DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_EN_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_CNT_500_EN_get", value );

    return value;
}
static INLINE void digi_m1_field_CNT_500_ROLLOVER_set( digi_m1_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_CNT_500_ROLLOVER_set( digi_m1_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_CNT_500_ROLLOVER_set", value, 127);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_CNT_500_ROLLOVER_set", value );

    /* (0x0001603c bits 20:14) bits 0:6 use field CNT_500_ROLLOVER of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
    digi_m1_reg_CAL_500_CFG_0_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_MSK,
                                         DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_CNT_500_ROLLOVER_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_CNT_500_ROLLOVER_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001603c bits 20:14) bits 0:6 use field CNT_500_ROLLOVER of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
    reg_value = digi_m1_reg_CAL_500_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_MSK) >> DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_CNT_500_ROLLOVER_get", value );

    return value;
}
static INLINE void digi_m1_field_range_CNT_500_ROLLOVER_set( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_CNT_500_ROLLOVER_set( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_CNT_500_ROLLOVER_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_CNT_500_ROLLOVER_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_CNT_500_ROLLOVER_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001603c bits 20:14) bits 0:6 use field CNT_500_ROLLOVER of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
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
        /* (0x0001603c bits 20:14) bits 0:6 use field CNT_500_ROLLOVER of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
        digi_m1_reg_CAL_500_CFG_0_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_OFF + subfield_offset),
                                             DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_CNT_500_ROLLOVER_get( digi_m1_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_CNT_500_ROLLOVER_get( digi_m1_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_CNT_500_ROLLOVER_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_CNT_500_ROLLOVER_get", stop_bit, 6 );
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
    /* (0x0001603c bits 20:14) bits 0:6 use field CNT_500_ROLLOVER of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
    reg_value = digi_m1_reg_CAL_500_CFG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_MSK)
                  >> DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_MSK, DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_CNT_500_ROLLOVER_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_SS_CAL_500_DLY_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_SS_CAL_500_DLY_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_SS_CAL_500_DLY_set", value, 127);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_SS_CAL_500_DLY_set", value );

    /* (0x0001603c bits 13:7) bits 0:6 use field SS_CAL_500_DLY of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
    digi_m1_reg_CAL_500_CFG_0_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_MSK,
                                         DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_SS_CAL_500_DLY_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_SS_CAL_500_DLY_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001603c bits 13:7) bits 0:6 use field SS_CAL_500_DLY of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
    reg_value = digi_m1_reg_CAL_500_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_MSK) >> DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_SS_CAL_500_DLY_get", value );

    return value;
}
static INLINE void digi_m1_field_range_SS_CAL_500_DLY_set( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_SS_CAL_500_DLY_set( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_CAL_500_DLY_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_CAL_500_DLY_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_CAL_500_DLY_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001603c bits 13:7) bits 0:6 use field SS_CAL_500_DLY of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
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
        /* (0x0001603c bits 13:7) bits 0:6 use field SS_CAL_500_DLY of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
        digi_m1_reg_CAL_500_CFG_0_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_OFF + subfield_offset),
                                             DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_SS_CAL_500_DLY_get( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_SS_CAL_500_DLY_get( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_CAL_500_DLY_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_CAL_500_DLY_get", stop_bit, 6 );
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
    /* (0x0001603c bits 13:7) bits 0:6 use field SS_CAL_500_DLY of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
    reg_value = digi_m1_reg_CAL_500_CFG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_MSK)
                  >> DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_MSK, DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_CAL_500_DLY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_SS_TOC_500_DLY_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_SS_TOC_500_DLY_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_SS_TOC_500_DLY_set", value, 127);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_SS_TOC_500_DLY_set", value );

    /* (0x0001603c bits 6:0) bits 0:6 use field SS_TOC_500_DLY of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
    digi_m1_reg_CAL_500_CFG_0_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_MSK,
                                         DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_SS_TOC_500_DLY_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_SS_TOC_500_DLY_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001603c bits 6:0) bits 0:6 use field SS_TOC_500_DLY of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
    reg_value = digi_m1_reg_CAL_500_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_MSK) >> DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_SS_TOC_500_DLY_get", value );

    return value;
}
static INLINE void digi_m1_field_range_SS_TOC_500_DLY_set( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_SS_TOC_500_DLY_set( digi_m1_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_TOC_500_DLY_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_TOC_500_DLY_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_TOC_500_DLY_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001603c bits 6:0) bits 0:6 use field SS_TOC_500_DLY of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
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
        /* (0x0001603c bits 6:0) bits 0:6 use field SS_TOC_500_DLY of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
        digi_m1_reg_CAL_500_CFG_0_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_OFF + subfield_offset),
                                             DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_SS_TOC_500_DLY_get( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_SS_TOC_500_DLY_get( digi_m1_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_TOC_500_DLY_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_TOC_500_DLY_get", stop_bit, 6 );
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
    /* (0x0001603c bits 6:0) bits 0:6 use field SS_TOC_500_DLY of register PMC_DIGI_M1_REG_CAL_500_CFG_0 */
    reg_value = digi_m1_reg_CAL_500_CFG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_MSK)
                  >> DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_MSK, DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_TOC_500_DLY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_SS_TOC_500_EN_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_SS_TOC_500_EN_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_SS_TOC_500_EN_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_SS_TOC_500_EN_set", value );

    /* (0x00016040 bits 9:0) bits 0:9 use field SS_TOC_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_1 */
    digi_m1_reg_CAL_500_CFG_1_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_MSK,
                                         DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_SS_TOC_500_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_SS_TOC_500_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016040 bits 9:0) bits 0:9 use field SS_TOC_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_1 */
    reg_value = digi_m1_reg_CAL_500_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_MSK) >> DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_SS_TOC_500_EN_get", value );

    return value;
}
static INLINE void digi_m1_field_range_SS_TOC_500_EN_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_SS_TOC_500_EN_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_TOC_500_EN_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_TOC_500_EN_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_TOC_500_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016040 bits 9:0) bits 0:9 use field SS_TOC_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_1 */
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
        /* (0x00016040 bits 9:0) bits 0:9 use field SS_TOC_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_1 */
        digi_m1_reg_CAL_500_CFG_1_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_OFF + subfield_offset),
                                             DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_SS_TOC_500_EN_get( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_SS_TOC_500_EN_get( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_TOC_500_EN_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_TOC_500_EN_get", stop_bit, 9 );
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
    /* (0x00016040 bits 9:0) bits 0:9 use field SS_TOC_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_1 */
    reg_value = digi_m1_reg_CAL_500_CFG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_MSK)
                  >> DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_MSK, DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_TOC_500_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_SS_CAL_500_EN_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_SS_CAL_500_EN_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 67108863)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_SS_CAL_500_EN_set", value, 67108863);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_SS_CAL_500_EN_set", value );

    /* (0x00016044 bits 25:0) bits 0:25 use field SS_CAL_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_2 */
    digi_m1_reg_CAL_500_CFG_2_field_set( b_ptr,
                                         h_ptr,
                                         DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_MSK,
                                         DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_OFF,
                                         value);
}

static INLINE UINT32 digi_m1_field_SS_CAL_500_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_SS_CAL_500_EN_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016044 bits 25:0) bits 0:25 use field SS_CAL_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_2 */
    reg_value = digi_m1_reg_CAL_500_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_MSK) >> DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_SS_CAL_500_EN_get", value );

    return value;
}
static INLINE void digi_m1_field_range_SS_CAL_500_EN_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_SS_CAL_500_EN_set( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_CAL_500_EN_set", stop_bit, start_bit );
    if (stop_bit > 25) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_CAL_500_EN_set", stop_bit, 25 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_CAL_500_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016044 bits 25:0) bits 0:25 use field SS_CAL_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_2 */
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
        if (stop_bit < 25) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 25;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00016044 bits 25:0) bits 0:25 use field SS_CAL_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_2 */
        digi_m1_reg_CAL_500_CFG_2_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_OFF + subfield_offset),
                                             DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_SS_CAL_500_EN_get( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_SS_CAL_500_EN_get( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_SS_CAL_500_EN_get", stop_bit, start_bit );
    if (stop_bit > 25) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_SS_CAL_500_EN_get", stop_bit, 25 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 25) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 25;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00016044 bits 25:0) bits 0:25 use field SS_CAL_500_EN of register PMC_DIGI_M1_REG_CAL_500_CFG_2 */
    reg_value = digi_m1_reg_CAL_500_CFG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_MSK)
                  >> DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_MSK, DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_SS_CAL_500_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_IRIG_CLKO_OEB_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_IRIG_CLKO_OEB_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_IRIG_CLKO_OEB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_IRIG_CLKO_OEB_set", value );

    /* (0x00016064 bits 2) field IRIG_CLKO_OEB of register PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS */
    digi_m1_reg_IRIG_IF_OUTPUT_ENS_field_set( b_ptr,
                                              h_ptr,
                                              DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_CLKO_OEB_MSK,
                                              DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_CLKO_OEB_OFF,
                                              value);
}

static INLINE UINT32 digi_m1_field_IRIG_CLKO_OEB_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_IRIG_CLKO_OEB_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016064 bits 2) field IRIG_CLKO_OEB of register PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS */
    reg_value = digi_m1_reg_IRIG_IF_OUTPUT_ENS_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_CLKO_OEB_MSK) >> DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_CLKO_OEB_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_IRIG_CLKO_OEB_get", value );

    return value;
}
static INLINE void digi_m1_field_IRIG_DATO_OEB_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_IRIG_DATO_OEB_set( digi_m1_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_IRIG_DATO_OEB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_IRIG_DATO_OEB_set", value );

    /* (0x00016064 bits 1) field IRIG_DATO_OEB of register PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS */
    digi_m1_reg_IRIG_IF_OUTPUT_ENS_field_set( b_ptr,
                                              h_ptr,
                                              DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_DATO_OEB_MSK,
                                              DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_DATO_OEB_OFF,
                                              value);
}

static INLINE UINT32 digi_m1_field_IRIG_DATO_OEB_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_IRIG_DATO_OEB_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016064 bits 1) field IRIG_DATO_OEB of register PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS */
    reg_value = digi_m1_reg_IRIG_IF_OUTPUT_ENS_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_DATO_OEB_MSK) >> DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_DATO_OEB_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_IRIG_DATO_OEB_get", value );

    return value;
}
static INLINE void digi_m1_field_ONE_PPSO_OEB_set( digi_m1_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_ONE_PPSO_OEB_set( digi_m1_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_ONE_PPSO_OEB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_ONE_PPSO_OEB_set", value );

    /* (0x00016064 bits 0) field ONE_PPSO_OEB of register PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS */
    digi_m1_reg_IRIG_IF_OUTPUT_ENS_field_set( b_ptr,
                                              h_ptr,
                                              DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_ONE_PPSO_OEB_MSK,
                                              DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_ONE_PPSO_OEB_OFF,
                                              value);
}

static INLINE UINT32 digi_m1_field_ONE_PPSO_OEB_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_ONE_PPSO_OEB_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016064 bits 0) field ONE_PPSO_OEB of register PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS */
    reg_value = digi_m1_reg_IRIG_IF_OUTPUT_ENS_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_ONE_PPSO_OEB_MSK) >> DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_ONE_PPSO_OEB_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_ONE_PPSO_OEB_get", value );

    return value;
}
static INLINE void digi_m1_field_OTU_RX_MFPO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_OTU_RX_MFPO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_OTU_RX_MFPO_SEL_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_OTU_RX_MFPO_SEL_set", value );

    /* (0x00016068 bits 27:16) bits 0:11 use field OTU_RX_MFPO_SEL of register PMC_DIGI_M1_REG_OTU_MFPO_SELECT */
    digi_m1_reg_OTU_MFPO_SELECT_field_set( b_ptr,
                                           h_ptr,
                                           DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_MSK,
                                           DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_OFF,
                                           value);
}

static INLINE UINT32 digi_m1_field_OTU_RX_MFPO_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_OTU_RX_MFPO_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016068 bits 27:16) bits 0:11 use field OTU_RX_MFPO_SEL of register PMC_DIGI_M1_REG_OTU_MFPO_SELECT */
    reg_value = digi_m1_reg_OTU_MFPO_SELECT_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_MSK) >> DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_OTU_RX_MFPO_SEL_get", value );

    return value;
}
static INLINE void digi_m1_field_range_OTU_RX_MFPO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_OTU_RX_MFPO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_OTU_RX_MFPO_SEL_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_OTU_RX_MFPO_SEL_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_OTU_RX_MFPO_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016068 bits 27:16) bits 0:11 use field OTU_RX_MFPO_SEL of register PMC_DIGI_M1_REG_OTU_MFPO_SELECT */
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
        /* (0x00016068 bits 27:16) bits 0:11 use field OTU_RX_MFPO_SEL of register PMC_DIGI_M1_REG_OTU_MFPO_SELECT */
        digi_m1_reg_OTU_MFPO_SELECT_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_OFF + subfield_offset),
                                               DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_OTU_RX_MFPO_SEL_get( digi_m1_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_OTU_RX_MFPO_SEL_get( digi_m1_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_OTU_RX_MFPO_SEL_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_OTU_RX_MFPO_SEL_get", stop_bit, 11 );
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
    /* (0x00016068 bits 27:16) bits 0:11 use field OTU_RX_MFPO_SEL of register PMC_DIGI_M1_REG_OTU_MFPO_SELECT */
    reg_value = digi_m1_reg_OTU_MFPO_SELECT_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_MSK)
                  >> DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_MSK, DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_OTU_RX_MFPO_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void digi_m1_field_OTU_TX_MFPO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_OTU_TX_MFPO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_OTU_TX_MFPO_SEL_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_OTU_TX_MFPO_SEL_set", value );

    /* (0x00016068 bits 11:0) bits 0:11 use field OTU_TX_MFPO_SEL of register PMC_DIGI_M1_REG_OTU_MFPO_SELECT */
    digi_m1_reg_OTU_MFPO_SELECT_field_set( b_ptr,
                                           h_ptr,
                                           DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_MSK,
                                           DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_OFF,
                                           value);
}

static INLINE UINT32 digi_m1_field_OTU_TX_MFPO_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_OTU_TX_MFPO_SEL_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016068 bits 11:0) bits 0:11 use field OTU_TX_MFPO_SEL of register PMC_DIGI_M1_REG_OTU_MFPO_SELECT */
    reg_value = digi_m1_reg_OTU_MFPO_SELECT_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_MSK) >> DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_OTU_TX_MFPO_SEL_get", value );

    return value;
}
static INLINE void digi_m1_field_range_OTU_TX_MFPO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_range_OTU_TX_MFPO_SEL_set( digi_m1_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_OTU_TX_MFPO_SEL_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_OTU_TX_MFPO_SEL_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_OTU_TX_MFPO_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016068 bits 11:0) bits 0:11 use field OTU_TX_MFPO_SEL of register PMC_DIGI_M1_REG_OTU_MFPO_SELECT */
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
        /* (0x00016068 bits 11:0) bits 0:11 use field OTU_TX_MFPO_SEL of register PMC_DIGI_M1_REG_OTU_MFPO_SELECT */
        digi_m1_reg_OTU_MFPO_SELECT_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_OFF + subfield_offset),
                                               DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 digi_m1_field_range_OTU_TX_MFPO_SEL_get( digi_m1_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_range_OTU_TX_MFPO_SEL_get( digi_m1_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "digi_m1_field_range_OTU_TX_MFPO_SEL_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "digi_m1_field_range_OTU_TX_MFPO_SEL_get", stop_bit, 11 );
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
    /* (0x00016068 bits 11:0) bits 0:11 use field OTU_TX_MFPO_SEL of register PMC_DIGI_M1_REG_OTU_MFPO_SELECT */
    reg_value = digi_m1_reg_OTU_MFPO_SELECT_read(  b_ptr, h_ptr);
    field_value = (reg_value & DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_MSK)
                  >> DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_MSK, DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "digi_m1_field_range_OTU_TX_MFPO_SEL_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void digi_m1_field_REQ_CAL_PG_349_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_REQ_CAL_PG_349_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_REQ_CAL_PG_349_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_REQ_CAL_PG_349_set", value );

    /* (0x00016020 bits 0) field REQ_CAL_PG_349 of register PMC_DIGI_M1_REG_CAL_349_REQ */
    digi_m1_reg_CAL_349_REQ_field_set( b_ptr,
                                       h_ptr,
                                       DIGI_M1_REG_CAL_349_REQ_BIT_REQ_CAL_PG_349_MSK,
                                       DIGI_M1_REG_CAL_349_REQ_BIT_REQ_CAL_PG_349_OFF,
                                       value);
}

static INLINE UINT32 digi_m1_field_REQ_CAL_PG_349_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_REQ_CAL_PG_349_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016020 bits 0) field REQ_CAL_PG_349 of register PMC_DIGI_M1_REG_CAL_349_REQ */
    reg_value = digi_m1_reg_CAL_349_REQ_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_349_REQ_BIT_REQ_CAL_PG_349_MSK) >> DIGI_M1_REG_CAL_349_REQ_BIT_REQ_CAL_PG_349_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_REQ_CAL_PG_349_get", value );

    return value;
}
static INLINE void digi_m1_field_REQ_CAL_PG_500_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void digi_m1_field_REQ_CAL_PG_500_set( digi_m1_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "digi_m1_field_REQ_CAL_PG_500_set", value, 1);
    IOLOG( "%s <= 0x%08x", "digi_m1_field_REQ_CAL_PG_500_set", value );

    /* (0x00016048 bits 0) field REQ_CAL_PG_500 of register PMC_DIGI_M1_REG_CAL_500_REQ */
    digi_m1_reg_CAL_500_REQ_field_set( b_ptr,
                                       h_ptr,
                                       DIGI_M1_REG_CAL_500_REQ_BIT_REQ_CAL_PG_500_MSK,
                                       DIGI_M1_REG_CAL_500_REQ_BIT_REQ_CAL_PG_500_OFF,
                                       value);
}

static INLINE UINT32 digi_m1_field_REQ_CAL_PG_500_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 digi_m1_field_REQ_CAL_PG_500_get( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00016048 bits 0) field REQ_CAL_PG_500 of register PMC_DIGI_M1_REG_CAL_500_REQ */
    reg_value = digi_m1_reg_CAL_500_REQ_read(  b_ptr, h_ptr);
    value = (reg_value & DIGI_M1_REG_CAL_500_REQ_BIT_REQ_CAL_PG_500_MSK) >> DIGI_M1_REG_CAL_500_REQ_BIT_REQ_CAL_PG_500_OFF;
    IOLOG( "%s -> 0x%08x", "digi_m1_field_REQ_CAL_PG_500_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_burstsize10
 * ==================================================================================
 */
static INLINE void digi_m1_lfield_EFUSE_DATA_get( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value[10] ) ALWAYS_INLINE;
static INLINE void digi_m1_lfield_EFUSE_DATA_get( digi_m1_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value[10] )
{
    IOLOG( "%s ", "digi_m1_lfield_EFUSE_DATA_get");
    digi_m1_reg_EFUSE_STAT_array_burst_read( b_ptr,
                                             h_ptr,
                                             0,
                                             10,
                                             (UINT32 *)value);
}

static INLINE UINT32 digi_m1_lfield_range_EFUSE_DATA_get( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 digi_m1_lfield_range_EFUSE_DATA_get( digi_m1_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "digi_m1_lfield_range_EFUSE_DATA_get", start_bit, stop_bit );
    }
    reg_value = digi_m1_reg_EFUSE_STAT_array_read( b_ptr,
                                                   h_ptr,
                                                   word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "digi_m1_lfield_range_EFUSE_DATA_get", start_bit, stop_bit, value );
    return value;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DIGI_M1_IO_INLINE_H */
