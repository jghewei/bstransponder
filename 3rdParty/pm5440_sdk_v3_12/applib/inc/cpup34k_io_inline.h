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
 *     and register accessor functions for the cpup34k block
 *****************************************************************************/
#ifndef _CPUP34K_IO_INLINE_H
#define _CPUP34K_IO_INLINE_H

#include "pmc_handle.h"
#include "cpup34k_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CPUP34K_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for cpup34k
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
    pmc_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} cpup34k_buffer_t;
static INLINE void cpup34k_buffer_init( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void cpup34k_buffer_init( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "cpup34k_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void cpup34k_buffer_flush( cpup34k_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void cpup34k_buffer_flush( cpup34k_buffer_t *b_ptr )
{
    IOLOG( "cpup34k_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 cpup34k_reg_read( cpup34k_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_read( cpup34k_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
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
static INLINE void cpup34k_reg_write( cpup34k_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_write( cpup34k_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
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

static INLINE void cpup34k_field_set( cpup34k_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_set( cpup34k_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
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

static INLINE void cpup34k_action_on_write_field_set( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_action_on_write_field_set( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
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

static INLINE void cpup34k_burst_read( cpup34k_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpup34k_burst_read( cpup34k_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
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

static INLINE void cpup34k_burst_write( cpup34k_buffer_t *b_ptr,
                                        pmc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void cpup34k_burst_write( cpup34k_buffer_t *b_ptr,
                                        pmc_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE cpup34k_poll( cpup34k_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cpup34k_poll( cpup34k_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
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
 *  register access functions for cpup34k
 * ==================================================================================
 */

static INLINE void cpup34k_reg_SOFT_RESET_EN_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_SOFT_RESET_EN_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_SOFT_RESET_EN_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_SOFT_RESET_EN,
                       value);
}

static INLINE void cpup34k_reg_SOFT_RESET_EN_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_SOFT_RESET_EN_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_SOFT_RESET_EN_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_SOFT_RESET_EN,
                       mask,
                       PMC_CPUP34K_REG_SOFT_RESET_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_SOFT_RESET_EN_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_SOFT_RESET_EN_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_SOFT_RESET_EN);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_SOFT_RESET_EN_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_BOOT_CTRL_write( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BOOT_CTRL_write( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_BOOT_CTRL_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BOOT_CTRL,
                       value);
}

static INLINE void cpup34k_reg_BOOT_CTRL_field_set( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BOOT_CTRL_field_set( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_BOOT_CTRL_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BOOT_CTRL,
                       mask,
                       PMC_CPUP34K_REG_BOOT_CTRL_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_BOOT_CTRL_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_BOOT_CTRL_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_BOOT_CTRL);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_BOOT_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_BOOT_ADR_write( cpup34k_buffer_t *b_ptr,
                                               pmc_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BOOT_ADR_write( cpup34k_buffer_t *b_ptr,
                                               pmc_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_BOOT_ADR_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BOOT_ADR,
                       value);
}

static INLINE void cpup34k_reg_BOOT_ADR_field_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BOOT_ADR_field_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_BOOT_ADR_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BOOT_ADR,
                       mask,
                       PMC_CPUP34K_REG_BOOT_ADR_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_BOOT_ADR_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_BOOT_ADR_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_BOOT_ADR);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_BOOT_ADR_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_BOOT_STAT_write( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BOOT_STAT_write( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_BOOT_STAT_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BOOT_STAT,
                       value);
}

static INLINE void cpup34k_reg_BOOT_STAT_field_set( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BOOT_STAT_field_set( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_BOOT_STAT_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BOOT_STAT,
                       mask,
                       PMC_CPUP34K_REG_BOOT_STAT_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_BOOT_STAT_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_BOOT_STAT_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_BOOT_STAT);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_BOOT_STAT_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_BOOT_INFO_write( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BOOT_INFO_write( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_BOOT_INFO_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BOOT_INFO,
                       value);
}

static INLINE void cpup34k_reg_BOOT_INFO_field_set( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BOOT_INFO_field_set( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_BOOT_INFO_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BOOT_INFO,
                       mask,
                       PMC_CPUP34K_REG_BOOT_INFO_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_BOOT_INFO_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_BOOT_INFO_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_BOOT_INFO);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_BOOT_INFO_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_APP_USER_0_write( cpup34k_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_USER_0_write( cpup34k_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_APP_USER_0_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_USER_0,
                       value);
}

static INLINE void cpup34k_reg_APP_USER_0_field_set( cpup34k_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_USER_0_field_set( cpup34k_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_APP_USER_0_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_USER_0,
                       mask,
                       PMC_CPUP34K_REG_APP_USER_0_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_APP_USER_0_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_APP_USER_0_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_APP_USER_0);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_APP_USER_0_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_APP_USER_1_write( cpup34k_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_USER_1_write( cpup34k_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_APP_USER_1_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_USER_1,
                       value);
}

static INLINE void cpup34k_reg_APP_USER_1_field_set( cpup34k_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_USER_1_field_set( cpup34k_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_APP_USER_1_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_USER_1,
                       mask,
                       PMC_CPUP34K_REG_APP_USER_1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_APP_USER_1_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_APP_USER_1_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_APP_USER_1);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_APP_USER_1_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG,
                       value);
}

static INLINE void cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG,
                       mask,
                       PMC_CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG,
                       value);
}

static INLINE void cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                        pmc_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                        pmc_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG,
                       mask,
                       PMC_CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG,
                       value);
}

static INLINE void cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG,
                       mask,
                       PMC_CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG,
                       value);
}

static INLINE void cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG,
                       mask,
                       PMC_CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG,
                       value);
}

static INLINE void cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG,
                       mask,
                       PMC_CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                                        pmc_handle_t *h_ptr,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_write( cpup34k_buffer_t *b_ptr,
                                                                        pmc_handle_t *h_ptr,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG,
                       value);
}

static INLINE void cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG,
                       mask,
                       PMC_CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_write( cpup34k_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_write( cpup34k_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG,
                       value);
}

static INLINE void cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG,
                       mask,
                       PMC_CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_write( cpup34k_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_write( cpup34k_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG,
                       value);
}

static INLINE void cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                        pmc_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_field_set( cpup34k_buffer_t *b_ptr,
                                                                        pmc_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG,
                       mask,
                       PMC_CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_SOFT_RESET_BITS_write( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_SOFT_RESET_BITS_write( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_SOFT_RESET_BITS_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_CPUP34K_REG_SOFT_RESET_BITS,
                       value);
}

static INLINE void cpup34k_reg_SOFT_RESET_BITS_field_set( cpup34k_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_SOFT_RESET_BITS_field_set( cpup34k_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_SOFT_RESET_BITS_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_CPUP34K_REG_SOFT_RESET_BITS,
                       mask,
                       PMC_CPUP34K_REG_SOFT_RESET_BITS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_SOFT_RESET_BITS_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_SOFT_RESET_BITS_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_CPUP34K_REG_SOFT_RESET_BITS);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_SOFT_RESET_BITS_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_APP_SCRATCH_0_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_0_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_APP_SCRATCH_0_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_0,
                       value);
}

static INLINE void cpup34k_reg_APP_SCRATCH_0_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_0_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_APP_SCRATCH_0_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_0,
                       mask,
                       PMC_CPUP34K_REG_APP_SCRATCH_0_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_APP_SCRATCH_0_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_APP_SCRATCH_0_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_APP_SCRATCH_0);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_APP_SCRATCH_0_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_APP_SCRATCH_1_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_1_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_APP_SCRATCH_1_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_1,
                       value);
}

static INLINE void cpup34k_reg_APP_SCRATCH_1_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_1_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_APP_SCRATCH_1_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_1,
                       mask,
                       PMC_CPUP34K_REG_APP_SCRATCH_1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_APP_SCRATCH_1_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_APP_SCRATCH_1_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_APP_SCRATCH_1);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_APP_SCRATCH_1_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_APP_SCRATCH_2_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_2_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_APP_SCRATCH_2_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_2,
                       value);
}

static INLINE void cpup34k_reg_APP_SCRATCH_2_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_2_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_APP_SCRATCH_2_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_2,
                       mask,
                       PMC_CPUP34K_REG_APP_SCRATCH_2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_APP_SCRATCH_2_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_APP_SCRATCH_2_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_APP_SCRATCH_2);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_APP_SCRATCH_2_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_APP_SCRATCH_3_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_3_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_APP_SCRATCH_3_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_3,
                       value);
}

static INLINE void cpup34k_reg_APP_SCRATCH_3_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_3_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_APP_SCRATCH_3_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_3,
                       mask,
                       PMC_CPUP34K_REG_APP_SCRATCH_3_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_APP_SCRATCH_3_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_APP_SCRATCH_3_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_APP_SCRATCH_3);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_APP_SCRATCH_3_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_APP_SCRATCH_4_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_4_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_APP_SCRATCH_4_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_4,
                       value);
}

static INLINE void cpup34k_reg_APP_SCRATCH_4_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_4_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_APP_SCRATCH_4_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_4,
                       mask,
                       PMC_CPUP34K_REG_APP_SCRATCH_4_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_APP_SCRATCH_4_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_APP_SCRATCH_4_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_APP_SCRATCH_4);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_APP_SCRATCH_4_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_APP_SCRATCH_5_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_5_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_APP_SCRATCH_5_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_5,
                       value);
}

static INLINE void cpup34k_reg_APP_SCRATCH_5_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_5_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_APP_SCRATCH_5_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_5,
                       mask,
                       PMC_CPUP34K_REG_APP_SCRATCH_5_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_APP_SCRATCH_5_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_APP_SCRATCH_5_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_APP_SCRATCH_5);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_APP_SCRATCH_5_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_APP_SCRATCH_6_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_6_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_APP_SCRATCH_6_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_6,
                       value);
}

static INLINE void cpup34k_reg_APP_SCRATCH_6_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_6_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_APP_SCRATCH_6_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_6,
                       mask,
                       PMC_CPUP34K_REG_APP_SCRATCH_6_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_APP_SCRATCH_6_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_APP_SCRATCH_6_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_APP_SCRATCH_6);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_APP_SCRATCH_6_read", reg_value);
    return reg_value;
}

static INLINE void cpup34k_reg_APP_SCRATCH_7_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_7_write( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cpup34k_reg_APP_SCRATCH_7_write", value );
    cpup34k_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_7,
                       value);
}

static INLINE void cpup34k_reg_APP_SCRATCH_7_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_reg_APP_SCRATCH_7_field_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cpup34k_reg_APP_SCRATCH_7_field_set", mask, ofs, value );
    cpup34k_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CPUP34K_REG_APP_SCRATCH_7,
                       mask,
                       PMC_CPUP34K_REG_APP_SCRATCH_7_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cpup34k_reg_APP_SCRATCH_7_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_APP_SCRATCH_7_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CPUP34K_REG_APP_SCRATCH_7);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_APP_SCRATCH_7_read", reg_value);
    return reg_value;
}

static INLINE UINT32 cpup34k_reg_BOOT_CAUSE_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_BOOT_CAUSE_REG_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_CPUP34K_REG_BOOT_CAUSE_REG);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_BOOT_CAUSE_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 cpup34k_reg_DEVICE_ID_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_DEVICE_ID_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_CPUP34K_REG_DEVICE_ID);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_DEVICE_ID_read", reg_value);
    return reg_value;
}

static INLINE UINT32 cpup34k_reg_DIAG_ID_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_reg_DIAG_ID_read( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cpup34k_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_CPUP34K_REG_DIAG_ID);

    IOLOG( "%s -> 0x%08x;", "cpup34k_reg_DIAG_ID_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void cpup34k_field_MIPS_SW_RESET_EN_set( cpup34k_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_MIPS_SW_RESET_EN_set( cpup34k_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_MIPS_SW_RESET_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_MIPS_SW_RESET_EN_set", value );

    /* (0x00000004 bits 30) field MIPS_SW_RESET_EN of register PMC_CPUP34K_REG_SOFT_RESET_EN */
    cpup34k_reg_SOFT_RESET_EN_field_set( b_ptr,
                                         h_ptr,
                                         CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RESET_EN_MSK,
                                         CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RESET_EN_OFF,
                                         value);
}

static INLINE UINT32 cpup34k_field_MIPS_SW_RESET_EN_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_MIPS_SW_RESET_EN_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 30) field MIPS_SW_RESET_EN of register PMC_CPUP34K_REG_SOFT_RESET_EN */
    reg_value = cpup34k_reg_SOFT_RESET_EN_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RESET_EN_MSK) >> CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RESET_EN_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_MIPS_SW_RESET_EN_get", value );

    return value;
}

static INLINE void cpup34k_field_NIC_SW_RST_EN_set( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_NIC_SW_RST_EN_set( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_NIC_SW_RST_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_NIC_SW_RST_EN_set", value );

    /* (0x00000004 bits 18) field NIC_SW_RST_EN of register PMC_CPUP34K_REG_SOFT_RESET_EN */
    cpup34k_reg_SOFT_RESET_EN_field_set( b_ptr,
                                         h_ptr,
                                         CPUP34K_REG_SOFT_RESET_EN_BIT_NIC_SW_RST_EN_MSK,
                                         CPUP34K_REG_SOFT_RESET_EN_BIT_NIC_SW_RST_EN_OFF,
                                         value);
}

static INLINE UINT32 cpup34k_field_NIC_SW_RST_EN_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_NIC_SW_RST_EN_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 18) field NIC_SW_RST_EN of register PMC_CPUP34K_REG_SOFT_RESET_EN */
    reg_value = cpup34k_reg_SOFT_RESET_EN_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_SOFT_RESET_EN_BIT_NIC_SW_RST_EN_MSK) >> CPUP34K_REG_SOFT_RESET_EN_BIT_NIC_SW_RST_EN_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_NIC_SW_RST_EN_get", value );

    return value;
}
static INLINE void cpup34k_field_MIPS_SW_RST_AUTOCLEAR_EN_set( cpup34k_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_MIPS_SW_RST_AUTOCLEAR_EN_set( cpup34k_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_MIPS_SW_RST_AUTOCLEAR_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_MIPS_SW_RST_AUTOCLEAR_EN_set", value );

    /* (0x00000004 bits 0) field MIPS_SW_RST_AUTOCLEAR_EN of register PMC_CPUP34K_REG_SOFT_RESET_EN */
    cpup34k_reg_SOFT_RESET_EN_field_set( b_ptr,
                                         h_ptr,
                                         CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RST_AUTOCLEAR_EN_MSK,
                                         CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RST_AUTOCLEAR_EN_OFF,
                                         value);
}

static INLINE UINT32 cpup34k_field_MIPS_SW_RST_AUTOCLEAR_EN_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_MIPS_SW_RST_AUTOCLEAR_EN_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field MIPS_SW_RST_AUTOCLEAR_EN of register PMC_CPUP34K_REG_SOFT_RESET_EN */
    reg_value = cpup34k_reg_SOFT_RESET_EN_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RST_AUTOCLEAR_EN_MSK) >> CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RST_AUTOCLEAR_EN_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_MIPS_SW_RST_AUTOCLEAR_EN_get", value );

    return value;
}
static INLINE void cpup34k_field_BOOT_ENABLE_set( cpup34k_buffer_t *b_ptr,
                                                  pmc_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_BOOT_ENABLE_set( cpup34k_buffer_t *b_ptr,
                                                  pmc_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_BOOT_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_BOOT_ENABLE_set", value );

    /* (0x00000054 bits 0) field BOOT_ENABLE of register PMC_CPUP34K_REG_BOOT_CTRL */
    cpup34k_reg_BOOT_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     CPUP34K_REG_BOOT_CTRL_BIT_BOOT_ENABLE_MSK,
                                     CPUP34K_REG_BOOT_CTRL_BIT_BOOT_ENABLE_OFF,
                                     value);
}

static INLINE UINT32 cpup34k_field_BOOT_ENABLE_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_BOOT_ENABLE_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000054 bits 0) field BOOT_ENABLE of register PMC_CPUP34K_REG_BOOT_CTRL */
    reg_value = cpup34k_reg_BOOT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_BOOT_CTRL_BIT_BOOT_ENABLE_MSK) >> CPUP34K_REG_BOOT_CTRL_BIT_BOOT_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_BOOT_ENABLE_get", value );

    return value;
}
static INLINE void cpup34k_field_BOOT_JMP_ADDR_set( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_BOOT_JMP_ADDR_set( cpup34k_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_BOOT_JMP_ADDR_set", value );

    /* (0x00000058 bits 31:0) bits 0:31 use field BOOT_JMP_ADDR of register PMC_CPUP34K_REG_BOOT_ADR */
    cpup34k_reg_BOOT_ADR_field_set( b_ptr,
                                    h_ptr,
                                    CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_MSK,
                                    CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_OFF,
                                    value);
}

static INLINE UINT32 cpup34k_field_BOOT_JMP_ADDR_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_BOOT_JMP_ADDR_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000058 bits 31:0) bits 0:31 use field BOOT_JMP_ADDR of register PMC_CPUP34K_REG_BOOT_ADR */
    reg_value = cpup34k_reg_BOOT_ADR_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_MSK) >> CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_BOOT_JMP_ADDR_get", value );

    return value;
}
static INLINE void cpup34k_field_range_BOOT_JMP_ADDR_set( cpup34k_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_BOOT_JMP_ADDR_set( cpup34k_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_BOOT_JMP_ADDR_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_BOOT_JMP_ADDR_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_BOOT_JMP_ADDR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000058 bits 31:0) bits 0:31 use field BOOT_JMP_ADDR of register PMC_CPUP34K_REG_BOOT_ADR */
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
        /* (0x00000058 bits 31:0) bits 0:31 use field BOOT_JMP_ADDR of register PMC_CPUP34K_REG_BOOT_ADR */
        cpup34k_reg_BOOT_ADR_field_set( b_ptr,
                                        h_ptr,
                                        subfield_mask << (CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_OFF + subfield_offset),
                                        CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_BOOT_JMP_ADDR_get( cpup34k_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_BOOT_JMP_ADDR_get( cpup34k_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_BOOT_JMP_ADDR_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_BOOT_JMP_ADDR_get", stop_bit, 31 );
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
    /* (0x00000058 bits 31:0) bits 0:31 use field BOOT_JMP_ADDR of register PMC_CPUP34K_REG_BOOT_ADR */
    reg_value = cpup34k_reg_BOOT_ADR_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_MSK)
                  >> CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_MSK, CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_BOOT_JMP_ADDR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_BOOT_FAIL_set( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_BOOT_FAIL_set( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_BOOT_FAIL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_BOOT_FAIL_set", value );

    /* (0x0000005c bits 1) field BOOT_FAIL of register PMC_CPUP34K_REG_BOOT_STAT */
    cpup34k_reg_BOOT_STAT_field_set( b_ptr,
                                     h_ptr,
                                     CPUP34K_REG_BOOT_STAT_BIT_BOOT_FAIL_MSK,
                                     CPUP34K_REG_BOOT_STAT_BIT_BOOT_FAIL_OFF,
                                     value);
}

static INLINE UINT32 cpup34k_field_BOOT_FAIL_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_BOOT_FAIL_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000005c bits 1) field BOOT_FAIL of register PMC_CPUP34K_REG_BOOT_STAT */
    reg_value = cpup34k_reg_BOOT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_BOOT_STAT_BIT_BOOT_FAIL_MSK) >> CPUP34K_REG_BOOT_STAT_BIT_BOOT_FAIL_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_BOOT_FAIL_get", value );

    return value;
}
static INLINE void cpup34k_field_BOOT_DONE_set( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_BOOT_DONE_set( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_BOOT_DONE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_BOOT_DONE_set", value );

    /* (0x0000005c bits 0) field BOOT_DONE of register PMC_CPUP34K_REG_BOOT_STAT */
    cpup34k_reg_BOOT_STAT_field_set( b_ptr,
                                     h_ptr,
                                     CPUP34K_REG_BOOT_STAT_BIT_BOOT_DONE_MSK,
                                     CPUP34K_REG_BOOT_STAT_BIT_BOOT_DONE_OFF,
                                     value);
}

static INLINE UINT32 cpup34k_field_BOOT_DONE_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_BOOT_DONE_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000005c bits 0) field BOOT_DONE of register PMC_CPUP34K_REG_BOOT_STAT */
    reg_value = cpup34k_reg_BOOT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_BOOT_STAT_BIT_BOOT_DONE_MSK) >> CPUP34K_REG_BOOT_STAT_BIT_BOOT_DONE_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_BOOT_DONE_get", value );

    return value;
}
static INLINE void cpup34k_field_BOOT_INFO_set( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_BOOT_INFO_set( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_BOOT_INFO_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_BOOT_INFO_set", value );

    /* (0x00000060 bits 15:0) bits 0:15 use field BOOT_INFO of register PMC_CPUP34K_REG_BOOT_INFO */
    cpup34k_reg_BOOT_INFO_field_set( b_ptr,
                                     h_ptr,
                                     CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_MSK,
                                     CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_OFF,
                                     value);
}

static INLINE UINT32 cpup34k_field_BOOT_INFO_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_BOOT_INFO_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 15:0) bits 0:15 use field BOOT_INFO of register PMC_CPUP34K_REG_BOOT_INFO */
    reg_value = cpup34k_reg_BOOT_INFO_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_MSK) >> CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_BOOT_INFO_get", value );

    return value;
}
static INLINE void cpup34k_field_range_BOOT_INFO_set( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_BOOT_INFO_set( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_BOOT_INFO_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_BOOT_INFO_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_BOOT_INFO_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000060 bits 15:0) bits 0:15 use field BOOT_INFO of register PMC_CPUP34K_REG_BOOT_INFO */
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
        /* (0x00000060 bits 15:0) bits 0:15 use field BOOT_INFO of register PMC_CPUP34K_REG_BOOT_INFO */
        cpup34k_reg_BOOT_INFO_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_OFF + subfield_offset),
                                         CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_BOOT_INFO_get( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_BOOT_INFO_get( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_BOOT_INFO_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_BOOT_INFO_get", stop_bit, 15 );
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
    /* (0x00000060 bits 15:0) bits 0:15 use field BOOT_INFO of register PMC_CPUP34K_REG_BOOT_INFO */
    reg_value = cpup34k_reg_BOOT_INFO_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_MSK)
                  >> CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_MSK, CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_BOOT_INFO_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_APP_USER0_set( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_APP_USER0_set( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_APP_USER0_set", value );

    /* (0x000001a4 bits 31:0) bits 0:31 use field APP_USER0 of register PMC_CPUP34K_REG_APP_USER_0 */
    cpup34k_reg_APP_USER_0_field_set( b_ptr,
                                      h_ptr,
                                      CPUP34K_REG_APP_USER_0_BIT_APP_USER0_MSK,
                                      CPUP34K_REG_APP_USER_0_BIT_APP_USER0_OFF,
                                      value);
}

static INLINE UINT32 cpup34k_field_APP_USER0_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_APP_USER0_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a4 bits 31:0) bits 0:31 use field APP_USER0 of register PMC_CPUP34K_REG_APP_USER_0 */
    reg_value = cpup34k_reg_APP_USER_0_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_APP_USER_0_BIT_APP_USER0_MSK) >> CPUP34K_REG_APP_USER_0_BIT_APP_USER0_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_APP_USER0_get", value );

    return value;
}
static INLINE void cpup34k_field_range_APP_USER0_set( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_APP_USER0_set( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_USER0_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_USER0_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_USER0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001a4 bits 31:0) bits 0:31 use field APP_USER0 of register PMC_CPUP34K_REG_APP_USER_0 */
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
        /* (0x000001a4 bits 31:0) bits 0:31 use field APP_USER0 of register PMC_CPUP34K_REG_APP_USER_0 */
        cpup34k_reg_APP_USER_0_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (CPUP34K_REG_APP_USER_0_BIT_APP_USER0_OFF + subfield_offset),
                                          CPUP34K_REG_APP_USER_0_BIT_APP_USER0_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_APP_USER0_get( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_APP_USER0_get( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_USER0_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_USER0_get", stop_bit, 31 );
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
    /* (0x000001a4 bits 31:0) bits 0:31 use field APP_USER0 of register PMC_CPUP34K_REG_APP_USER_0 */
    reg_value = cpup34k_reg_APP_USER_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_APP_USER_0_BIT_APP_USER0_MSK)
                  >> CPUP34K_REG_APP_USER_0_BIT_APP_USER0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_APP_USER_0_BIT_APP_USER0_MSK, CPUP34K_REG_APP_USER_0_BIT_APP_USER0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_USER0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_APP_USER1_set( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_APP_USER1_set( cpup34k_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_APP_USER1_set", value );

    /* (0x000001a8 bits 31:0) bits 0:31 use field APP_USER1 of register PMC_CPUP34K_REG_APP_USER_1 */
    cpup34k_reg_APP_USER_1_field_set( b_ptr,
                                      h_ptr,
                                      CPUP34K_REG_APP_USER_1_BIT_APP_USER1_MSK,
                                      CPUP34K_REG_APP_USER_1_BIT_APP_USER1_OFF,
                                      value);
}

static INLINE UINT32 cpup34k_field_APP_USER1_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_APP_USER1_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 31:0) bits 0:31 use field APP_USER1 of register PMC_CPUP34K_REG_APP_USER_1 */
    reg_value = cpup34k_reg_APP_USER_1_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_APP_USER_1_BIT_APP_USER1_MSK) >> CPUP34K_REG_APP_USER_1_BIT_APP_USER1_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_APP_USER1_get", value );

    return value;
}
static INLINE void cpup34k_field_range_APP_USER1_set( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_APP_USER1_set( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_USER1_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_USER1_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_USER1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001a8 bits 31:0) bits 0:31 use field APP_USER1 of register PMC_CPUP34K_REG_APP_USER_1 */
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
        /* (0x000001a8 bits 31:0) bits 0:31 use field APP_USER1 of register PMC_CPUP34K_REG_APP_USER_1 */
        cpup34k_reg_APP_USER_1_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (CPUP34K_REG_APP_USER_1_BIT_APP_USER1_OFF + subfield_offset),
                                          CPUP34K_REG_APP_USER_1_BIT_APP_USER1_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_APP_USER1_get( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_APP_USER1_get( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_USER1_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_USER1_get", stop_bit, 31 );
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
    /* (0x000001a8 bits 31:0) bits 0:31 use field APP_USER1 of register PMC_CPUP34K_REG_APP_USER_1 */
    reg_value = cpup34k_reg_APP_USER_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_APP_USER_1_BIT_APP_USER1_MSK)
                  >> CPUP34K_REG_APP_USER_1_BIT_APP_USER1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_APP_USER_1_BIT_APP_USER1_MSK, CPUP34K_REG_APP_USER_1_BIT_APP_USER1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_USER1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_BASE_ADD_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_BASE_ADD_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_BASE_ADD_INBOUND_MAILBOX_set", value );

    /* (0x000001ac bits 31:0) bits 0:31 use field BASE_ADD_INBOUND_MAILBOX of register PMC_CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG */
    cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_field_set( b_ptr,
                                                        h_ptr,
                                                        CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_MSK,
                                                        CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_OFF,
                                                        value);
}

static INLINE UINT32 cpup34k_field_BASE_ADD_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_BASE_ADD_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001ac bits 31:0) bits 0:31 use field BASE_ADD_INBOUND_MAILBOX of register PMC_CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_MSK) >> CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_BASE_ADD_INBOUND_MAILBOX_get", value );

    return value;
}
static INLINE void cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001ac bits 31:0) bits 0:31 use field BASE_ADD_INBOUND_MAILBOX of register PMC_CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG */
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
        /* (0x000001ac bits 31:0) bits 0:31 use field BASE_ADD_INBOUND_MAILBOX of register PMC_CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG */
        cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_OFF + subfield_offset),
                                                            CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_get", stop_bit, 31 );
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
    /* (0x000001ac bits 31:0) bits 0:31 use field BASE_ADD_INBOUND_MAILBOX of register PMC_CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_MSK)
                  >> CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_MSK, CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_BASE_ADD_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_BASE_ADD_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_BASE_ADD_OUTBOUND_MAILBOX_set", value );

    /* (0x000001b0 bits 31:0) bits 0:31 use field BASE_ADD_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG */
    cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_MSK,
                                                         CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_OFF,
                                                         value);
}

static INLINE UINT32 cpup34k_field_BASE_ADD_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_BASE_ADD_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b0 bits 31:0) bits 0:31 use field BASE_ADD_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_MSK) >> CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_BASE_ADD_OUTBOUND_MAILBOX_get", value );

    return value;
}
static INLINE void cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                      pmc_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                      pmc_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001b0 bits 31:0) bits 0:31 use field BASE_ADD_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG */
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
        /* (0x000001b0 bits 31:0) bits 0:31 use field BASE_ADD_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG */
        cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_field_set( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_OFF + subfield_offset),
                                                             CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                        pmc_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                        pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_get", stop_bit, 31 );
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
    /* (0x000001b0 bits 31:0) bits 0:31 use field BASE_ADD_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_read( b_ptr,
                                                                h_ptr);
    field_value = (reg_value & CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_MSK)
                  >> CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_MSK, CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_SIZE_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_SIZE_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_SIZE_INBOUND_MAILBOX_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_SIZE_INBOUND_MAILBOX_set", value );

    /* (0x000001b4 bits 15:0) bits 0:15 use field SIZE_INBOUND_MAILBOX of register PMC_CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG */
    cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_MSK,
                                                    CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_OFF,
                                                    value);
}

static INLINE UINT32 cpup34k_field_SIZE_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_SIZE_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b4 bits 15:0) bits 0:15 use field SIZE_INBOUND_MAILBOX of register PMC_CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_MSK) >> CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_SIZE_INBOUND_MAILBOX_get", value );

    return value;
}
static INLINE void cpup34k_field_range_SIZE_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_SIZE_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_SIZE_INBOUND_MAILBOX_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_SIZE_INBOUND_MAILBOX_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_SIZE_INBOUND_MAILBOX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001b4 bits 15:0) bits 0:15 use field SIZE_INBOUND_MAILBOX of register PMC_CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG */
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
        /* (0x000001b4 bits 15:0) bits 0:15 use field SIZE_INBOUND_MAILBOX of register PMC_CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG */
        cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_OFF + subfield_offset),
                                                        CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_SIZE_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_SIZE_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_SIZE_INBOUND_MAILBOX_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_SIZE_INBOUND_MAILBOX_get", stop_bit, 15 );
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
    /* (0x000001b4 bits 15:0) bits 0:15 use field SIZE_INBOUND_MAILBOX of register PMC_CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_MSK)
                  >> CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_MSK, CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_SIZE_INBOUND_MAILBOX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_SIZE_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_SIZE_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_SIZE_OUTBOUND_MAILBOX_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_SIZE_OUTBOUND_MAILBOX_set", value );

    /* (0x000001b8 bits 15:0) bits 0:15 use field SIZE_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG */
    cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_MSK,
                                                     CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_OFF,
                                                     value);
}

static INLINE UINT32 cpup34k_field_SIZE_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_SIZE_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 15:0) bits 0:15 use field SIZE_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_MSK) >> CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_SIZE_OUTBOUND_MAILBOX_get", value );

    return value;
}
static INLINE void cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                  pmc_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                  pmc_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001b8 bits 15:0) bits 0:15 use field SIZE_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG */
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
        /* (0x000001b8 bits 15:0) bits 0:15 use field SIZE_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG */
        cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_OFF + subfield_offset),
                                                         CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_get", stop_bit, 15 );
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
    /* (0x000001b8 bits 15:0) bits 0:15 use field SIZE_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_MSK)
                  >> CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_MSK, CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_MAX_MESSAGES_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_MAX_MESSAGES_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_MAX_MESSAGES_INBOUND_MAILBOX_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_MAX_MESSAGES_INBOUND_MAILBOX_set", value );

    /* (0x000001bc bits 15:0) bits 0:15 use field MAX_MESSAGES_INBOUND_MAILBOX of register PMC_CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG */
    cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_MSK,
                                                            CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_OFF,
                                                            value);
}

static INLINE UINT32 cpup34k_field_MAX_MESSAGES_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_MAX_MESSAGES_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001bc bits 15:0) bits 0:15 use field MAX_MESSAGES_INBOUND_MAILBOX of register PMC_CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_MSK) >> CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_MAX_MESSAGES_INBOUND_MAILBOX_get", value );

    return value;
}
static INLINE void cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001bc bits 15:0) bits 0:15 use field MAX_MESSAGES_INBOUND_MAILBOX of register PMC_CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG */
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
        /* (0x000001bc bits 15:0) bits 0:15 use field MAX_MESSAGES_INBOUND_MAILBOX of register PMC_CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG */
        cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_OFF + subfield_offset),
                                                                CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_get", stop_bit, 15 );
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
    /* (0x000001bc bits 15:0) bits 0:15 use field MAX_MESSAGES_INBOUND_MAILBOX of register PMC_CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_MSK)
                  >> CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_MSK, CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_MAX_MESSAGES_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_MAX_MESSAGES_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_MAX_MESSAGES_OUTBOUND_MAILBOX_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_MAX_MESSAGES_OUTBOUND_MAILBOX_set", value );

    /* (0x000001c0 bits 15:0) bits 0:15 use field MAX_MESSAGES_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG */
    cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_field_set( b_ptr,
                                                             h_ptr,
                                                             CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_MSK,
                                                             CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_OFF,
                                                             value);
}

static INLINE UINT32 cpup34k_field_MAX_MESSAGES_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_MAX_MESSAGES_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001c0 bits 15:0) bits 0:15 use field MAX_MESSAGES_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_read( b_ptr,
                                                                    h_ptr);
    value = (reg_value & CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_MSK) >> CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_MAX_MESSAGES_OUTBOUND_MAILBOX_get", value );

    return value;
}
static INLINE void cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_set( cpup34k_buffer_t *b_ptr,
                                                                          pmc_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001c0 bits 15:0) bits 0:15 use field MAX_MESSAGES_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG */
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
        /* (0x000001c0 bits 15:0) bits 0:15 use field MAX_MESSAGES_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG */
        cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_OFF + subfield_offset),
                                                                 CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_get( cpup34k_buffer_t *b_ptr,
                                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_get", stop_bit, 15 );
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
    /* (0x000001c0 bits 15:0) bits 0:15 use field MAX_MESSAGES_OUTBOUND_MAILBOX of register PMC_CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG */
    reg_value = cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_read( b_ptr,
                                                                    h_ptr);
    field_value = (reg_value & CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_MSK)
                  >> CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_MSK, CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_MAX_SIZE_INBOUND_MESSAGE_set( cpup34k_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_MAX_SIZE_INBOUND_MESSAGE_set( cpup34k_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_MAX_SIZE_INBOUND_MESSAGE_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_MAX_SIZE_INBOUND_MESSAGE_set", value );

    /* (0x000001c4 bits 15:0) bits 0:15 use field MAX_SIZE_INBOUND_MESSAGE of register PMC_CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG */
    cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_field_set( b_ptr,
                                                        h_ptr,
                                                        CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_MSK,
                                                        CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_OFF,
                                                        value);
}

static INLINE UINT32 cpup34k_field_MAX_SIZE_INBOUND_MESSAGE_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_MAX_SIZE_INBOUND_MESSAGE_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001c4 bits 15:0) bits 0:15 use field MAX_SIZE_INBOUND_MESSAGE of register PMC_CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG */
    reg_value = cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_MSK) >> CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_MAX_SIZE_INBOUND_MESSAGE_get", value );

    return value;
}
static INLINE void cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_set( cpup34k_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_set( cpup34k_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001c4 bits 15:0) bits 0:15 use field MAX_SIZE_INBOUND_MESSAGE of register PMC_CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG */
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
        /* (0x000001c4 bits 15:0) bits 0:15 use field MAX_SIZE_INBOUND_MESSAGE of register PMC_CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG */
        cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_OFF + subfield_offset),
                                                            CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_get( cpup34k_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_get( cpup34k_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_get", stop_bit, 15 );
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
    /* (0x000001c4 bits 15:0) bits 0:15 use field MAX_SIZE_INBOUND_MESSAGE of register PMC_CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG */
    reg_value = cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_MSK)
                  >> CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_MSK, CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_MAX_SIZE_OUTBOUND_MESSAGE_set( cpup34k_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_MAX_SIZE_OUTBOUND_MESSAGE_set( cpup34k_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_MAX_SIZE_OUTBOUND_MESSAGE_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_MAX_SIZE_OUTBOUND_MESSAGE_set", value );

    /* (0x000001c8 bits 15:0) bits 0:15 use field MAX_SIZE_OUTBOUND_MESSAGE of register PMC_CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG */
    cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_MSK,
                                                         CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_OFF,
                                                         value);
}

static INLINE UINT32 cpup34k_field_MAX_SIZE_OUTBOUND_MESSAGE_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_MAX_SIZE_OUTBOUND_MESSAGE_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001c8 bits 15:0) bits 0:15 use field MAX_SIZE_OUTBOUND_MESSAGE of register PMC_CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG */
    reg_value = cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_MSK) >> CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_MAX_SIZE_OUTBOUND_MESSAGE_get", value );

    return value;
}
static INLINE void cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_set( cpup34k_buffer_t *b_ptr,
                                                                      pmc_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_set( cpup34k_buffer_t *b_ptr,
                                                                      pmc_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001c8 bits 15:0) bits 0:15 use field MAX_SIZE_OUTBOUND_MESSAGE of register PMC_CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG */
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
        /* (0x000001c8 bits 15:0) bits 0:15 use field MAX_SIZE_OUTBOUND_MESSAGE of register PMC_CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG */
        cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_field_set( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_OFF + subfield_offset),
                                                             CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_get( cpup34k_buffer_t *b_ptr,
                                                                        pmc_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_get( cpup34k_buffer_t *b_ptr,
                                                                        pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_get", stop_bit, 15 );
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
    /* (0x000001c8 bits 15:0) bits 0:15 use field MAX_SIZE_OUTBOUND_MESSAGE of register PMC_CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG */
    reg_value = cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_read( b_ptr,
                                                                h_ptr);
    field_value = (reg_value & CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_MSK)
                  >> CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_MSK, CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void cpup34k_field_MIPS_SW_RST_set( cpup34k_buffer_t *b_ptr,
                                                  pmc_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_MIPS_SW_RST_set( cpup34k_buffer_t *b_ptr,
                                                  pmc_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_MIPS_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_MIPS_SW_RST_set", value );

    /* (0x00000000 bits 30) field MIPS_SW_RST of register PMC_CPUP34K_REG_SOFT_RESET_BITS */
    cpup34k_reg_SOFT_RESET_BITS_field_set( b_ptr,
                                           h_ptr,
                                           CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_MSK,
                                           CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_OFF,
                                           value);
}

static INLINE UINT32 cpup34k_field_MIPS_SW_RST_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_MIPS_SW_RST_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 30) field MIPS_SW_RST of register PMC_CPUP34K_REG_SOFT_RESET_BITS */
    reg_value = cpup34k_reg_SOFT_RESET_BITS_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_MSK) >> CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_MIPS_SW_RST_get", value );

    return value;
}


static INLINE void cpup34k_field_NIC_SW_RST_set( cpup34k_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_NIC_SW_RST_set( cpup34k_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_NIC_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_NIC_SW_RST_set", value );

    /* (0x00000000 bits 18) field NIC_SW_RST of register PMC_CPUP34K_REG_SOFT_RESET_BITS */
    cpup34k_reg_SOFT_RESET_BITS_field_set( b_ptr,
                                           h_ptr,
                                           CPUP34K_REG_SOFT_RESET_BITS_BIT_NIC_SW_RST_MSK,
                                           CPUP34K_REG_SOFT_RESET_BITS_BIT_NIC_SW_RST_OFF,
                                           value);
}

static INLINE UINT32 cpup34k_field_NIC_SW_RST_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_NIC_SW_RST_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 18) field NIC_SW_RST of register PMC_CPUP34K_REG_SOFT_RESET_BITS */
    reg_value = cpup34k_reg_SOFT_RESET_BITS_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_SOFT_RESET_BITS_BIT_NIC_SW_RST_MSK) >> CPUP34K_REG_SOFT_RESET_BITS_BIT_NIC_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_NIC_SW_RST_get", value );

    return value;
}
static INLINE void cpup34k_field_MIPS_SW_RST_CLEAR_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_MIPS_SW_RST_CLEAR_set( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cpup34k_field_MIPS_SW_RST_CLEAR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cpup34k_field_MIPS_SW_RST_CLEAR_set", value );

    /* (0x00000000 bits 0) field MIPS_SW_RST_CLEAR of register PMC_CPUP34K_REG_SOFT_RESET_BITS */
    cpup34k_reg_SOFT_RESET_BITS_field_set( b_ptr,
                                           h_ptr,
                                           CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_CLEAR_MSK,
                                           CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_CLEAR_OFF,
                                           value);
}

static INLINE UINT32 cpup34k_field_MIPS_SW_RST_CLEAR_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_MIPS_SW_RST_CLEAR_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field MIPS_SW_RST_CLEAR of register PMC_CPUP34K_REG_SOFT_RESET_BITS */
    reg_value = cpup34k_reg_SOFT_RESET_BITS_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_CLEAR_MSK) >> CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_CLEAR_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_MIPS_SW_RST_CLEAR_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset scratch
 * ==================================================================================
 */
static INLINE void cpup34k_field_APP_SCRATCH0_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_APP_SCRATCH0_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_APP_SCRATCH0_set", value );

    /* (0x00000184 bits 31:0) bits 0:31 use field APP_SCRATCH0 of register PMC_CPUP34K_REG_APP_SCRATCH_0 */
    cpup34k_reg_APP_SCRATCH_0_field_set( b_ptr,
                                         h_ptr,
                                         CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_MSK,
                                         CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_OFF,
                                         value);
}

static INLINE UINT32 cpup34k_field_APP_SCRATCH0_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_APP_SCRATCH0_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000184 bits 31:0) bits 0:31 use field APP_SCRATCH0 of register PMC_CPUP34K_REG_APP_SCRATCH_0 */
    reg_value = cpup34k_reg_APP_SCRATCH_0_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_MSK) >> CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_APP_SCRATCH0_get", value );

    return value;
}
static INLINE void cpup34k_field_range_APP_SCRATCH0_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_APP_SCRATCH0_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH0_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH0_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000184 bits 31:0) bits 0:31 use field APP_SCRATCH0 of register PMC_CPUP34K_REG_APP_SCRATCH_0 */
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
        /* (0x00000184 bits 31:0) bits 0:31 use field APP_SCRATCH0 of register PMC_CPUP34K_REG_APP_SCRATCH_0 */
        cpup34k_reg_APP_SCRATCH_0_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_OFF + subfield_offset),
                                             CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_APP_SCRATCH0_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_APP_SCRATCH0_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH0_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH0_get", stop_bit, 31 );
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
    /* (0x00000184 bits 31:0) bits 0:31 use field APP_SCRATCH0 of register PMC_CPUP34K_REG_APP_SCRATCH_0 */
    reg_value = cpup34k_reg_APP_SCRATCH_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_MSK)
                  >> CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_MSK, CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_APP_SCRATCH1_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_APP_SCRATCH1_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_APP_SCRATCH1_set", value );

    /* (0x00000188 bits 31:0) bits 0:31 use field APP_SCRATCH1 of register PMC_CPUP34K_REG_APP_SCRATCH_1 */
    cpup34k_reg_APP_SCRATCH_1_field_set( b_ptr,
                                         h_ptr,
                                         CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_MSK,
                                         CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_OFF,
                                         value);
}

static INLINE UINT32 cpup34k_field_APP_SCRATCH1_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_APP_SCRATCH1_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 31:0) bits 0:31 use field APP_SCRATCH1 of register PMC_CPUP34K_REG_APP_SCRATCH_1 */
    reg_value = cpup34k_reg_APP_SCRATCH_1_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_MSK) >> CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_APP_SCRATCH1_get", value );

    return value;
}
static INLINE void cpup34k_field_range_APP_SCRATCH1_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_APP_SCRATCH1_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH1_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH1_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000188 bits 31:0) bits 0:31 use field APP_SCRATCH1 of register PMC_CPUP34K_REG_APP_SCRATCH_1 */
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
        /* (0x00000188 bits 31:0) bits 0:31 use field APP_SCRATCH1 of register PMC_CPUP34K_REG_APP_SCRATCH_1 */
        cpup34k_reg_APP_SCRATCH_1_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_OFF + subfield_offset),
                                             CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_APP_SCRATCH1_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_APP_SCRATCH1_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH1_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH1_get", stop_bit, 31 );
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
    /* (0x00000188 bits 31:0) bits 0:31 use field APP_SCRATCH1 of register PMC_CPUP34K_REG_APP_SCRATCH_1 */
    reg_value = cpup34k_reg_APP_SCRATCH_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_MSK)
                  >> CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_MSK, CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_APP_SCRATCH2_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_APP_SCRATCH2_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_APP_SCRATCH2_set", value );

    /* (0x0000018c bits 31:0) bits 0:31 use field APP_SCRATCH2 of register PMC_CPUP34K_REG_APP_SCRATCH_2 */
    cpup34k_reg_APP_SCRATCH_2_field_set( b_ptr,
                                         h_ptr,
                                         CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_MSK,
                                         CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_OFF,
                                         value);
}

static INLINE UINT32 cpup34k_field_APP_SCRATCH2_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_APP_SCRATCH2_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000018c bits 31:0) bits 0:31 use field APP_SCRATCH2 of register PMC_CPUP34K_REG_APP_SCRATCH_2 */
    reg_value = cpup34k_reg_APP_SCRATCH_2_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_MSK) >> CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_APP_SCRATCH2_get", value );

    return value;
}
static INLINE void cpup34k_field_range_APP_SCRATCH2_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_APP_SCRATCH2_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH2_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH2_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000018c bits 31:0) bits 0:31 use field APP_SCRATCH2 of register PMC_CPUP34K_REG_APP_SCRATCH_2 */
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
        /* (0x0000018c bits 31:0) bits 0:31 use field APP_SCRATCH2 of register PMC_CPUP34K_REG_APP_SCRATCH_2 */
        cpup34k_reg_APP_SCRATCH_2_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_OFF + subfield_offset),
                                             CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_APP_SCRATCH2_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_APP_SCRATCH2_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH2_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH2_get", stop_bit, 31 );
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
    /* (0x0000018c bits 31:0) bits 0:31 use field APP_SCRATCH2 of register PMC_CPUP34K_REG_APP_SCRATCH_2 */
    reg_value = cpup34k_reg_APP_SCRATCH_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_MSK)
                  >> CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_MSK, CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_APP_SCRATCH3_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_APP_SCRATCH3_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_APP_SCRATCH3_set", value );

    /* (0x00000190 bits 31:0) bits 0:31 use field APP_SCRATCH3 of register PMC_CPUP34K_REG_APP_SCRATCH_3 */
    cpup34k_reg_APP_SCRATCH_3_field_set( b_ptr,
                                         h_ptr,
                                         CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_MSK,
                                         CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_OFF,
                                         value);
}

static INLINE UINT32 cpup34k_field_APP_SCRATCH3_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_APP_SCRATCH3_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000190 bits 31:0) bits 0:31 use field APP_SCRATCH3 of register PMC_CPUP34K_REG_APP_SCRATCH_3 */
    reg_value = cpup34k_reg_APP_SCRATCH_3_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_MSK) >> CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_APP_SCRATCH3_get", value );

    return value;
}
static INLINE void cpup34k_field_range_APP_SCRATCH3_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_APP_SCRATCH3_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH3_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH3_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000190 bits 31:0) bits 0:31 use field APP_SCRATCH3 of register PMC_CPUP34K_REG_APP_SCRATCH_3 */
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
        /* (0x00000190 bits 31:0) bits 0:31 use field APP_SCRATCH3 of register PMC_CPUP34K_REG_APP_SCRATCH_3 */
        cpup34k_reg_APP_SCRATCH_3_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_OFF + subfield_offset),
                                             CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_APP_SCRATCH3_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_APP_SCRATCH3_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH3_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH3_get", stop_bit, 31 );
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
    /* (0x00000190 bits 31:0) bits 0:31 use field APP_SCRATCH3 of register PMC_CPUP34K_REG_APP_SCRATCH_3 */
    reg_value = cpup34k_reg_APP_SCRATCH_3_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_MSK)
                  >> CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_MSK, CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_APP_SCRATCH4_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_APP_SCRATCH4_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_APP_SCRATCH4_set", value );

    /* (0x00000194 bits 31:0) bits 0:31 use field APP_SCRATCH4 of register PMC_CPUP34K_REG_APP_SCRATCH_4 */
    cpup34k_reg_APP_SCRATCH_4_field_set( b_ptr,
                                         h_ptr,
                                         CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_MSK,
                                         CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_OFF,
                                         value);
}

static INLINE UINT32 cpup34k_field_APP_SCRATCH4_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_APP_SCRATCH4_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000194 bits 31:0) bits 0:31 use field APP_SCRATCH4 of register PMC_CPUP34K_REG_APP_SCRATCH_4 */
    reg_value = cpup34k_reg_APP_SCRATCH_4_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_MSK) >> CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_APP_SCRATCH4_get", value );

    return value;
}
static INLINE void cpup34k_field_range_APP_SCRATCH4_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_APP_SCRATCH4_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH4_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH4_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH4_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000194 bits 31:0) bits 0:31 use field APP_SCRATCH4 of register PMC_CPUP34K_REG_APP_SCRATCH_4 */
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
        /* (0x00000194 bits 31:0) bits 0:31 use field APP_SCRATCH4 of register PMC_CPUP34K_REG_APP_SCRATCH_4 */
        cpup34k_reg_APP_SCRATCH_4_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_OFF + subfield_offset),
                                             CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_APP_SCRATCH4_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_APP_SCRATCH4_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH4_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH4_get", stop_bit, 31 );
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
    /* (0x00000194 bits 31:0) bits 0:31 use field APP_SCRATCH4 of register PMC_CPUP34K_REG_APP_SCRATCH_4 */
    reg_value = cpup34k_reg_APP_SCRATCH_4_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_MSK)
                  >> CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_MSK, CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_APP_SCRATCH5_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_APP_SCRATCH5_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_APP_SCRATCH5_set", value );

    /* (0x00000198 bits 31:0) bits 0:31 use field APP_SCRATCH5 of register PMC_CPUP34K_REG_APP_SCRATCH_5 */
    cpup34k_reg_APP_SCRATCH_5_field_set( b_ptr,
                                         h_ptr,
                                         CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_MSK,
                                         CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_OFF,
                                         value);
}

static INLINE UINT32 cpup34k_field_APP_SCRATCH5_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_APP_SCRATCH5_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 31:0) bits 0:31 use field APP_SCRATCH5 of register PMC_CPUP34K_REG_APP_SCRATCH_5 */
    reg_value = cpup34k_reg_APP_SCRATCH_5_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_MSK) >> CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_APP_SCRATCH5_get", value );

    return value;
}
static INLINE void cpup34k_field_range_APP_SCRATCH5_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_APP_SCRATCH5_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH5_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH5_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH5_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000198 bits 31:0) bits 0:31 use field APP_SCRATCH5 of register PMC_CPUP34K_REG_APP_SCRATCH_5 */
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
        /* (0x00000198 bits 31:0) bits 0:31 use field APP_SCRATCH5 of register PMC_CPUP34K_REG_APP_SCRATCH_5 */
        cpup34k_reg_APP_SCRATCH_5_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_OFF + subfield_offset),
                                             CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_APP_SCRATCH5_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_APP_SCRATCH5_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH5_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH5_get", stop_bit, 31 );
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
    /* (0x00000198 bits 31:0) bits 0:31 use field APP_SCRATCH5 of register PMC_CPUP34K_REG_APP_SCRATCH_5 */
    reg_value = cpup34k_reg_APP_SCRATCH_5_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_MSK)
                  >> CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_MSK, CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH5_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_APP_SCRATCH6_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_APP_SCRATCH6_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_APP_SCRATCH6_set", value );

    /* (0x0000019c bits 31:0) bits 0:31 use field APP_SCRATCH6 of register PMC_CPUP34K_REG_APP_SCRATCH_6 */
    cpup34k_reg_APP_SCRATCH_6_field_set( b_ptr,
                                         h_ptr,
                                         CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_MSK,
                                         CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_OFF,
                                         value);
}

static INLINE UINT32 cpup34k_field_APP_SCRATCH6_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_APP_SCRATCH6_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000019c bits 31:0) bits 0:31 use field APP_SCRATCH6 of register PMC_CPUP34K_REG_APP_SCRATCH_6 */
    reg_value = cpup34k_reg_APP_SCRATCH_6_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_MSK) >> CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_APP_SCRATCH6_get", value );

    return value;
}
static INLINE void cpup34k_field_range_APP_SCRATCH6_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_APP_SCRATCH6_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH6_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH6_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH6_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000019c bits 31:0) bits 0:31 use field APP_SCRATCH6 of register PMC_CPUP34K_REG_APP_SCRATCH_6 */
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
        /* (0x0000019c bits 31:0) bits 0:31 use field APP_SCRATCH6 of register PMC_CPUP34K_REG_APP_SCRATCH_6 */
        cpup34k_reg_APP_SCRATCH_6_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_OFF + subfield_offset),
                                             CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_APP_SCRATCH6_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_APP_SCRATCH6_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH6_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH6_get", stop_bit, 31 );
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
    /* (0x0000019c bits 31:0) bits 0:31 use field APP_SCRATCH6 of register PMC_CPUP34K_REG_APP_SCRATCH_6 */
    reg_value = cpup34k_reg_APP_SCRATCH_6_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_MSK)
                  >> CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_MSK, CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void cpup34k_field_APP_SCRATCH7_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_APP_SCRATCH7_set( cpup34k_buffer_t *b_ptr,
                                                   pmc_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "cpup34k_field_APP_SCRATCH7_set", value );

    /* (0x000001a0 bits 31:0) bits 0:31 use field APP_SCRATCH7 of register PMC_CPUP34K_REG_APP_SCRATCH_7 */
    cpup34k_reg_APP_SCRATCH_7_field_set( b_ptr,
                                         h_ptr,
                                         CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_MSK,
                                         CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_OFF,
                                         value);
}

static INLINE UINT32 cpup34k_field_APP_SCRATCH7_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_APP_SCRATCH7_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a0 bits 31:0) bits 0:31 use field APP_SCRATCH7 of register PMC_CPUP34K_REG_APP_SCRATCH_7 */
    reg_value = cpup34k_reg_APP_SCRATCH_7_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_MSK) >> CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_APP_SCRATCH7_get", value );

    return value;
}
static INLINE void cpup34k_field_range_APP_SCRATCH7_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cpup34k_field_range_APP_SCRATCH7_set( cpup34k_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH7_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH7_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH7_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001a0 bits 31:0) bits 0:31 use field APP_SCRATCH7 of register PMC_CPUP34K_REG_APP_SCRATCH_7 */
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
        /* (0x000001a0 bits 31:0) bits 0:31 use field APP_SCRATCH7 of register PMC_CPUP34K_REG_APP_SCRATCH_7 */
        cpup34k_reg_APP_SCRATCH_7_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_OFF + subfield_offset),
                                             CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 cpup34k_field_range_APP_SCRATCH7_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_APP_SCRATCH7_get( cpup34k_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_APP_SCRATCH7_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_APP_SCRATCH7_get", stop_bit, 31 );
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
    /* (0x000001a0 bits 31:0) bits 0:31 use field APP_SCRATCH7 of register PMC_CPUP34K_REG_APP_SCRATCH_7 */
    reg_value = cpup34k_reg_APP_SCRATCH_7_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_MSK)
                  >> CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_MSK, CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_APP_SCRATCH7_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 cpup34k_field_CAUSE_MIPS_SELF_RST_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_CAUSE_MIPS_SELF_RST_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000068 bits 3) field CAUSE_MIPS_SELF_RST of register PMC_CPUP34K_REG_BOOT_CAUSE_REG */
    reg_value = cpup34k_reg_BOOT_CAUSE_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_MIPS_SELF_RST_MSK) >> CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_MIPS_SELF_RST_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_CAUSE_MIPS_SELF_RST_get", value );

    return value;
}
static INLINE UINT32 cpup34k_field_CAUSE_WDOG0_TIMEOUT_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_CAUSE_WDOG0_TIMEOUT_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000068 bits 2) field CAUSE_WDOG0_TIMEOUT of register PMC_CPUP34K_REG_BOOT_CAUSE_REG */
    reg_value = cpup34k_reg_BOOT_CAUSE_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_WDOG0_TIMEOUT_MSK) >> CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_WDOG0_TIMEOUT_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_CAUSE_WDOG0_TIMEOUT_get", value );

    return value;
}
static INLINE UINT32 cpup34k_field_CAUSE_WDOG1_TIMEOUT_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_CAUSE_WDOG1_TIMEOUT_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000068 bits 1) field CAUSE_WDOG1_TIMEOUT of register PMC_CPUP34K_REG_BOOT_CAUSE_REG */
    reg_value = cpup34k_reg_BOOT_CAUSE_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_WDOG1_TIMEOUT_MSK) >> CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_WDOG1_TIMEOUT_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_CAUSE_WDOG1_TIMEOUT_get", value );

    return value;
}
static INLINE UINT32 cpup34k_field_CAUSE_MIPS_SW_RST_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_CAUSE_MIPS_SW_RST_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000068 bits 0) field CAUSE_MIPS_SW_RST of register PMC_CPUP34K_REG_BOOT_CAUSE_REG */
    reg_value = cpup34k_reg_BOOT_CAUSE_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_MIPS_SW_RST_MSK) >> CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_MIPS_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_CAUSE_MIPS_SW_RST_get", value );

    return value;
}
static INLINE UINT32 cpup34k_field_REVISION_CODE_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_REVISION_CODE_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000180 bits 31:28) bits 0:3 use field REVISION_CODE of register PMC_CPUP34K_REG_DEVICE_ID */
    reg_value = cpup34k_reg_DEVICE_ID_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_DEVICE_ID_BIT_REVISION_CODE_MSK) >> CPUP34K_REG_DEVICE_ID_BIT_REVISION_CODE_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_REVISION_CODE_get", value );

    return value;
}
static INLINE UINT32 cpup34k_field_range_REVISION_CODE_get( cpup34k_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_REVISION_CODE_get( cpup34k_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_REVISION_CODE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_REVISION_CODE_get", stop_bit, 3 );
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
    /* (0x00000180 bits 31:28) bits 0:3 use field REVISION_CODE of register PMC_CPUP34K_REG_DEVICE_ID */
    reg_value = cpup34k_reg_DEVICE_ID_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_DEVICE_ID_BIT_REVISION_CODE_MSK)
                  >> CPUP34K_REG_DEVICE_ID_BIT_REVISION_CODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_DEVICE_ID_BIT_REVISION_CODE_MSK, CPUP34K_REG_DEVICE_ID_BIT_REVISION_CODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_REVISION_CODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 cpup34k_field_DEVICE_ID_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_DEVICE_ID_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000180 bits 27:12) bits 0:15 use field DEVICE_ID of register PMC_CPUP34K_REG_DEVICE_ID */
    reg_value = cpup34k_reg_DEVICE_ID_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_DEVICE_ID_BIT_DEVICE_ID_MSK) >> CPUP34K_REG_DEVICE_ID_BIT_DEVICE_ID_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_DEVICE_ID_get", value );

    return value;
}
static INLINE UINT32 cpup34k_field_range_DEVICE_ID_get( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_DEVICE_ID_get( cpup34k_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_DEVICE_ID_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_DEVICE_ID_get", stop_bit, 15 );
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
    /* (0x00000180 bits 27:12) bits 0:15 use field DEVICE_ID of register PMC_CPUP34K_REG_DEVICE_ID */
    reg_value = cpup34k_reg_DEVICE_ID_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_DEVICE_ID_BIT_DEVICE_ID_MSK)
                  >> CPUP34K_REG_DEVICE_ID_BIT_DEVICE_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_DEVICE_ID_BIT_DEVICE_ID_MSK, CPUP34K_REG_DEVICE_ID_BIT_DEVICE_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_DEVICE_ID_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 cpup34k_field_PMC_ID_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_PMC_ID_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000180 bits 11:0) bits 0:11 use field PMC_ID of register PMC_CPUP34K_REG_DEVICE_ID */
    reg_value = cpup34k_reg_DEVICE_ID_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_DEVICE_ID_BIT_PMC_ID_MSK) >> CPUP34K_REG_DEVICE_ID_BIT_PMC_ID_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_PMC_ID_get", value );

    return value;
}
static INLINE UINT32 cpup34k_field_range_PMC_ID_get( cpup34k_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_PMC_ID_get( cpup34k_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_PMC_ID_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_PMC_ID_get", stop_bit, 11 );
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
    /* (0x00000180 bits 11:0) bits 0:11 use field PMC_ID of register PMC_CPUP34K_REG_DEVICE_ID */
    reg_value = cpup34k_reg_DEVICE_ID_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_DEVICE_ID_BIT_PMC_ID_MSK)
                  >> CPUP34K_REG_DEVICE_ID_BIT_PMC_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_DEVICE_ID_BIT_PMC_ID_MSK, CPUP34K_REG_DEVICE_ID_BIT_PMC_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_PMC_ID_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 cpup34k_field_DIAG_ID_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_DIAG_ID_get( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001d0 bits 31:0) bits 0:31 use field DIAG_ID of register PMC_CPUP34K_REG_DIAG_ID */
    reg_value = cpup34k_reg_DIAG_ID_read(  b_ptr, h_ptr);
    value = (reg_value & CPUP34K_REG_DIAG_ID_BIT_DIAG_ID_MSK) >> CPUP34K_REG_DIAG_ID_BIT_DIAG_ID_OFF;
    IOLOG( "%s -> 0x%08x", "cpup34k_field_DIAG_ID_get", value );

    return value;
}
static INLINE UINT32 cpup34k_field_range_DIAG_ID_get( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cpup34k_field_range_DIAG_ID_get( cpup34k_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cpup34k_field_range_DIAG_ID_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cpup34k_field_range_DIAG_ID_get", stop_bit, 31 );
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
    /* (0x000001d0 bits 31:0) bits 0:31 use field DIAG_ID of register PMC_CPUP34K_REG_DIAG_ID */
    reg_value = cpup34k_reg_DIAG_ID_read(  b_ptr, h_ptr);
    field_value = (reg_value & CPUP34K_REG_DIAG_ID_BIT_DIAG_ID_MSK)
                  >> CPUP34K_REG_DIAG_ID_BIT_DIAG_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CPUP34K_REG_DIAG_ID_BIT_DIAG_ID_MSK, CPUP34K_REG_DIAG_ID_BIT_DIAG_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "cpup34k_field_range_DIAG_ID_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CPUP34K_IO_INLINE_H */
