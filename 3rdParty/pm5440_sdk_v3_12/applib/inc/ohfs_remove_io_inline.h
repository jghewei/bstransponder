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
 *     and register accessor functions for the ohfs_remove block
 *****************************************************************************/
#ifndef _OHFS_REMOVE_IO_INLINE_H
#define _OHFS_REMOVE_IO_INLINE_H

#include "ohfs_remove.h"
#include "ohfs_remove_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OHFS_REMOVE_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for ohfs_remove
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
    ohfs_remove_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} ohfs_remove_buffer_t;
static INLINE void ohfs_remove_buffer_init( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void ohfs_remove_buffer_init( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "ohfs_remove_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void ohfs_remove_buffer_flush( ohfs_remove_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void ohfs_remove_buffer_flush( ohfs_remove_buffer_t *b_ptr )
{
    IOLOG( "ohfs_remove_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 ohfs_remove_reg_read( ohfs_remove_buffer_t *b_ptr,
                                           ohfs_remove_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_read( ohfs_remove_buffer_t *b_ptr,
                                           ohfs_remove_handle_t *h_ptr,
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
static INLINE void ohfs_remove_reg_write( ohfs_remove_buffer_t *b_ptr,
                                          ohfs_remove_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_write( ohfs_remove_buffer_t *b_ptr,
                                          ohfs_remove_handle_t *h_ptr,
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

static INLINE void ohfs_remove_field_set( ohfs_remove_buffer_t *b_ptr,
                                          ohfs_remove_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 mask,
                                          UINT32 unused_mask,
                                          UINT32 ofs,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_set( ohfs_remove_buffer_t *b_ptr,
                                          ohfs_remove_handle_t *h_ptr,
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

static INLINE void ohfs_remove_action_on_write_field_set( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32 mem_type,
                                                          UINT32 reg,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_action_on_write_field_set( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
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

static INLINE void ohfs_remove_burst_read( ohfs_remove_buffer_t *b_ptr,
                                           ohfs_remove_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_burst_read( ohfs_remove_buffer_t *b_ptr,
                                           ohfs_remove_handle_t *h_ptr,
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

static INLINE void ohfs_remove_burst_write( ohfs_remove_buffer_t *b_ptr,
                                            ohfs_remove_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_burst_write( ohfs_remove_buffer_t *b_ptr,
                                            ohfs_remove_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE ohfs_remove_poll( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 value,
                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                     UINT32 max_count,
                                                     UINT32 *num_failed_polls,
                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE ohfs_remove_poll( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
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
 *  register access functions for ohfs_remove
 * ==================================================================================
 */

static INLINE void ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_write( ohfs_remove_buffer_t *b_ptr,
                                                                     ohfs_remove_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_write( ohfs_remove_buffer_t *b_ptr,
                                                                     ohfs_remove_handle_t *h_ptr,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_write", value );
    ohfs_remove_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG,
                           value);
}

static INLINE void ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                         ohfs_remove_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                         ohfs_remove_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_field_set", mask, ofs, value );
    ohfs_remove_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG,
                           mask,
                           PMC_OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_read( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_read( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ohfs_remove_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG);

    IOLOG( "%s -> 0x%08x;", "ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_read", reg_value);
    return reg_value;
}

static INLINE void ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_write( ohfs_remove_buffer_t *b_ptr,
                                                                      ohfs_remove_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_write( ohfs_remove_buffer_t *b_ptr,
                                                                      ohfs_remove_handle_t *h_ptr,
                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_write", value );
    ohfs_remove_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG,
                           value);
}

static INLINE void ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                          ohfs_remove_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                          ohfs_remove_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_field_set", mask, ofs, value );
    ohfs_remove_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG,
                           mask,
                           PMC_OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_read( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_read( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ohfs_remove_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG);

    IOLOG( "%s -> 0x%08x;", "ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_read", reg_value);
    return reg_value;
}

static INLINE void ohfs_remove_reg_TS_ADDRESS_IPT_array_write( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_TS_ADDRESS_IPT_array_write( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_remove_reg_TS_ADDRESS_IPT_array_write", value );
    ohfs_remove_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT(N),
                           value);
}

static INLINE void ohfs_remove_reg_TS_ADDRESS_IPT_array_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                   ohfs_remove_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_TS_ADDRESS_IPT_array_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                   ohfs_remove_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_remove_reg_TS_ADDRESS_IPT_array_field_set", N, mask, ofs, value );
    ohfs_remove_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT(N),
                           mask,
                           PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_remove_reg_TS_ADDRESS_IPT_array_read( ohfs_remove_buffer_t *b_ptr,
                                                                ohfs_remove_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_TS_ADDRESS_IPT_array_read( ohfs_remove_buffer_t *b_ptr,
                                                                ohfs_remove_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_remove_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_remove_reg_TS_ADDRESS_IPT_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_remove_reg_RCOH_STAT_LATCH_CFG_write( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_RCOH_STAT_LATCH_CFG_write( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_remove_reg_RCOH_STAT_LATCH_CFG_write", value );
    ohfs_remove_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG,
                           value);
}

static INLINE void ohfs_remove_reg_RCOH_STAT_LATCH_CFG_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                  ohfs_remove_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_RCOH_STAT_LATCH_CFG_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                  ohfs_remove_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "ohfs_remove_reg_RCOH_STAT_LATCH_CFG_field_set", mask, ofs, value );
    ohfs_remove_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG,
                           mask,
                           PMC_OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_remove_reg_RCOH_STAT_LATCH_CFG_read( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_RCOH_STAT_LATCH_CFG_read( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = ohfs_remove_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG);

    IOLOG( "%s -> 0x%08x;", "ohfs_remove_reg_RCOH_STAT_LATCH_CFG_read", reg_value);
    return reg_value;
}

static INLINE void ohfs_remove_reg_COMMON_CFG_array_write( ohfs_remove_buffer_t *b_ptr,
                                                           ohfs_remove_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_COMMON_CFG_array_write( ohfs_remove_buffer_t *b_ptr,
                                                           ohfs_remove_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_remove_reg_COMMON_CFG_array_write", value );
    ohfs_remove_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_REMOVE_REG_COMMON_CFG(N),
                           value);
}

static INLINE void ohfs_remove_reg_COMMON_CFG_array_field_set( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_COMMON_CFG_array_field_set( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_remove_reg_COMMON_CFG_array_field_set", N, mask, ofs, value );
    ohfs_remove_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_REMOVE_REG_COMMON_CFG(N),
                           mask,
                           PMC_OHFS_REMOVE_REG_COMMON_CFG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_remove_reg_COMMON_CFG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_COMMON_CFG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_remove_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_REMOVE_REG_COMMON_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_remove_reg_COMMON_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_write( ohfs_remove_buffer_t *b_ptr,
                                                                                   ohfs_remove_handle_t *h_ptr,
                                                                                   UINT32  N,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_write( ohfs_remove_buffer_t *b_ptr,
                                                                                   ohfs_remove_handle_t *h_ptr,
                                                                                   UINT32  N,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_write", value );
    ohfs_remove_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION(N),
                           value);
}

static INLINE void ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                                       ohfs_remove_handle_t *h_ptr,
                                                                                       UINT32  N,
                                                                                       UINT32 mask,
                                                                                       UINT32 ofs,
                                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                                       ohfs_remove_handle_t *h_ptr,
                                                                                       UINT32  N,
                                                                                       UINT32 mask,
                                                                                       UINT32 ofs,
                                                                                       UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set", N, mask, ofs, value );
    ohfs_remove_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION(N),
                           mask,
                           PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read( ohfs_remove_buffer_t *b_ptr,
                                                                                    ohfs_remove_handle_t *h_ptr,
                                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read( ohfs_remove_buffer_t *b_ptr,
                                                                                    ohfs_remove_handle_t *h_ptr,
                                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_remove_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_write( ohfs_remove_buffer_t *b_ptr,
                                                                    ohfs_remove_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_write( ohfs_remove_buffer_t *b_ptr,
                                                                    ohfs_remove_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_write", value );
    ohfs_remove_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG(N),
                           value);
}

static INLINE void ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                        ohfs_remove_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                        ohfs_remove_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_field_set", N, mask, ofs, value );
    ohfs_remove_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG(N),
                           mask,
                           PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                                     ohfs_remove_handle_t *h_ptr,
                                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                                     ohfs_remove_handle_t *h_ptr,
                                                                     UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_remove_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_remove_reg_LO_ID_MSI_CFG_array_write( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_LO_ID_MSI_CFG_array_write( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_remove_reg_LO_ID_MSI_CFG_array_write", value );
    ohfs_remove_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG(N),
                           value);
}

static INLINE void ohfs_remove_reg_LO_ID_MSI_CFG_array_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                  ohfs_remove_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_reg_LO_ID_MSI_CFG_array_field_set( ohfs_remove_buffer_t *b_ptr,
                                                                  ohfs_remove_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_remove_reg_LO_ID_MSI_CFG_array_field_set", N, mask, ofs, value );
    ohfs_remove_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG(N),
                           mask,
                           PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_remove_reg_LO_ID_MSI_CFG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_LO_ID_MSI_CFG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_remove_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_remove_reg_LO_ID_MSI_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 ohfs_remove_reg_PT_STAT_REG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                             ohfs_remove_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_PT_STAT_REG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                             ohfs_remove_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_remove_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_REMOVE_REG_PT_STAT_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_remove_reg_PT_STAT_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 ohfs_remove_reg_MSI_STAT_REG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_MSI_STAT_REG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_remove_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_REMOVE_REG_MSI_STAT_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_remove_reg_MSI_STAT_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 ohfs_remove_reg_RCOH_STAT_REG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_reg_RCOH_STAT_REG_array_read( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_remove_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_REMOVE_REG_RCOH_STAT_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_remove_reg_RCOH_STAT_REG_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void ohfs_remove_field_PT_STAT_UPDATE_DISABLE_set( ohfs_remove_buffer_t *b_ptr,
                                                                 ohfs_remove_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_PT_STAT_UPDATE_DISABLE_set( ohfs_remove_buffer_t *b_ptr,
                                                                 ohfs_remove_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_PT_STAT_UPDATE_DISABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ohfs_remove_field_PT_STAT_UPDATE_DISABLE_set", value );

    /* (0x00000000 bits 0) field PT_STAT_UPDATE_DISABLE of register PMC_OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG */
    ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_field_set( b_ptr,
                                                          h_ptr,
                                                          OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG_BIT_PT_STAT_UPDATE_DISABLE_MSK,
                                                          OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG_BIT_PT_STAT_UPDATE_DISABLE_OFF,
                                                          value);
}

static INLINE UINT32 ohfs_remove_field_PT_STAT_UPDATE_DISABLE_get( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_PT_STAT_UPDATE_DISABLE_get( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field PT_STAT_UPDATE_DISABLE of register PMC_OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG */
    reg_value = ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG_BIT_PT_STAT_UPDATE_DISABLE_MSK) >> OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG_BIT_PT_STAT_UPDATE_DISABLE_OFF;
    IOLOG( "%s -> 0x%08x", "ohfs_remove_field_PT_STAT_UPDATE_DISABLE_get", value );

    return value;
}
static INLINE void ohfs_remove_field_MSI_STAT_UPDATE_DISABLE_set( ohfs_remove_buffer_t *b_ptr,
                                                                  ohfs_remove_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_MSI_STAT_UPDATE_DISABLE_set( ohfs_remove_buffer_t *b_ptr,
                                                                  ohfs_remove_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_MSI_STAT_UPDATE_DISABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ohfs_remove_field_MSI_STAT_UPDATE_DISABLE_set", value );

    /* (0x00000004 bits 0) field MSI_STAT_UPDATE_DISABLE of register PMC_OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG */
    ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_field_set( b_ptr,
                                                           h_ptr,
                                                           OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG_BIT_MSI_STAT_UPDATE_DISABLE_MSK,
                                                           OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG_BIT_MSI_STAT_UPDATE_DISABLE_OFF,
                                                           value);
}

static INLINE UINT32 ohfs_remove_field_MSI_STAT_UPDATE_DISABLE_get( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_MSI_STAT_UPDATE_DISABLE_get( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field MSI_STAT_UPDATE_DISABLE of register PMC_OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG */
    reg_value = ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG_BIT_MSI_STAT_UPDATE_DISABLE_MSK) >> OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG_BIT_MSI_STAT_UPDATE_DISABLE_OFF;
    IOLOG( "%s -> 0x%08x", "ohfs_remove_field_MSI_STAT_UPDATE_DISABLE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void ohfs_remove_field_TS_IPT_FRM_NUM_set( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_TS_IPT_FRM_NUM_set( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_TS_IPT_FRM_NUM_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_TS_IPT_FRM_NUM_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_TS_IPT_FRM_NUM_set", N, value );

    /* ((0x00000800 + (N) * 0x10) bits 22:16) field TS_IPT_FRM_NUM of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
    ohfs_remove_reg_TS_ADDRESS_IPT_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_MSK,
                                                    OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_OFF,
                                                    value);
}

static INLINE UINT32 ohfs_remove_field_TS_IPT_FRM_NUM_get( ohfs_remove_buffer_t *b_ptr,
                                                           ohfs_remove_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_TS_IPT_FRM_NUM_get( ohfs_remove_buffer_t *b_ptr,
                                                           ohfs_remove_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_TS_IPT_FRM_NUM_get", N, 95);
    /* ((0x00000800 + (N) * 0x10) bits 22:16) field TS_IPT_FRM_NUM of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
    reg_value = ohfs_remove_reg_TS_ADDRESS_IPT_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_MSK) >> OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_TS_IPT_FRM_NUM_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_range_TS_IPT_FRM_NUM_set( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_range_TS_IPT_FRM_NUM_set( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_TS_IPT_FRM_NUM_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_TS_IPT_FRM_NUM_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_TS_IPT_FRM_NUM_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_TS_IPT_FRM_NUM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000800 + (N) * 0x10) bits 22:16) field TS_IPT_FRM_NUM of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
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
        /* ((0x00000800 + (N) * 0x10) bits 22:16) field TS_IPT_FRM_NUM of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
        ohfs_remove_reg_TS_ADDRESS_IPT_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_OFF + subfield_offset),
                                                        OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_remove_field_range_TS_IPT_FRM_NUM_get( ohfs_remove_buffer_t *b_ptr,
                                                                 ohfs_remove_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_TS_IPT_FRM_NUM_get( ohfs_remove_buffer_t *b_ptr,
                                                                 ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_TS_IPT_FRM_NUM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_TS_IPT_FRM_NUM_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_TS_IPT_FRM_NUM_get", stop_bit, 6 );
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
    /* ((0x00000800 + (N) * 0x10) bits 22:16) field TS_IPT_FRM_NUM of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
    reg_value = ohfs_remove_reg_TS_ADDRESS_IPT_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_MSK)
                  >> OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_MSK, OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_TS_IPT_FRM_NUM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_remove_field_TS_IPT_ID_set( ohfs_remove_buffer_t *b_ptr,
                                                    ohfs_remove_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_TS_IPT_ID_set( ohfs_remove_buffer_t *b_ptr,
                                                    ohfs_remove_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_TS_IPT_ID_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_TS_IPT_ID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_TS_IPT_ID_set", N, value );

    /* ((0x00000800 + (N) * 0x10) bits 14:8) field TS_IPT_ID of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
    ohfs_remove_reg_TS_ADDRESS_IPT_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_MSK,
                                                    OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_OFF,
                                                    value);
}

static INLINE UINT32 ohfs_remove_field_TS_IPT_ID_get( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_TS_IPT_ID_get( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_TS_IPT_ID_get", N, 95);
    /* ((0x00000800 + (N) * 0x10) bits 14:8) field TS_IPT_ID of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
    reg_value = ohfs_remove_reg_TS_ADDRESS_IPT_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_MSK) >> OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_TS_IPT_ID_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_range_TS_IPT_ID_set( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_range_TS_IPT_ID_set( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_TS_IPT_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_TS_IPT_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_TS_IPT_ID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_TS_IPT_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000800 + (N) * 0x10) bits 14:8) field TS_IPT_ID of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
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
        /* ((0x00000800 + (N) * 0x10) bits 14:8) field TS_IPT_ID of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
        ohfs_remove_reg_TS_ADDRESS_IPT_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_OFF + subfield_offset),
                                                        OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_remove_field_range_TS_IPT_ID_get( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_TS_IPT_ID_get( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_TS_IPT_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_TS_IPT_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_TS_IPT_ID_get", stop_bit, 6 );
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
    /* ((0x00000800 + (N) * 0x10) bits 14:8) field TS_IPT_ID of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
    reg_value = ohfs_remove_reg_TS_ADDRESS_IPT_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_MSK)
                  >> OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_MSK, OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_TS_IPT_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_remove_field_TS_IPT_VALID_set( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_TS_IPT_VALID_set( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_TS_IPT_VALID_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_TS_IPT_VALID_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_TS_IPT_VALID_set", N, value );

    /* ((0x00000800 + (N) * 0x10) bits 0) field TS_IPT_VALID of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
    ohfs_remove_reg_TS_ADDRESS_IPT_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_VALID_MSK,
                                                    OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_VALID_OFF,
                                                    value);
}

static INLINE UINT32 ohfs_remove_field_TS_IPT_VALID_get( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_TS_IPT_VALID_get( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_TS_IPT_VALID_get", N, 95);
    /* ((0x00000800 + (N) * 0x10) bits 0) field TS_IPT_VALID of register PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT index N=0..95 */
    reg_value = ohfs_remove_reg_TS_ADDRESS_IPT_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_VALID_MSK) >> OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_VALID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_TS_IPT_VALID_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void ohfs_remove_field_TRNSFR_RCOH_TO_SHADOW_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                                   ohfs_remove_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_TRNSFR_RCOH_TO_SHADOW_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                                   ohfs_remove_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_TRNSFR_RCOH_TO_SHADOW_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "ohfs_remove_field_TRNSFR_RCOH_TO_SHADOW_EN_set", value );

    /* (0x00000008 bits 0) field TRNSFR_RCOH_TO_SHADOW_EN of register PMC_OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG */
    ohfs_remove_reg_RCOH_STAT_LATCH_CFG_field_set( b_ptr,
                                                   h_ptr,
                                                   OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG_BIT_TRNSFR_RCOH_TO_SHADOW_EN_MSK,
                                                   OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG_BIT_TRNSFR_RCOH_TO_SHADOW_EN_OFF,
                                                   value);
}

static INLINE UINT32 ohfs_remove_field_TRNSFR_RCOH_TO_SHADOW_EN_get( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_TRNSFR_RCOH_TO_SHADOW_EN_get( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 0) field TRNSFR_RCOH_TO_SHADOW_EN of register PMC_OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG */
    reg_value = ohfs_remove_reg_RCOH_STAT_LATCH_CFG_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG_BIT_TRNSFR_RCOH_TO_SHADOW_EN_MSK) >> OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG_BIT_TRNSFR_RCOH_TO_SHADOW_EN_OFF;
    IOLOG( "%s -> 0x%08x", "ohfs_remove_field_TRNSFR_RCOH_TO_SHADOW_EN_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void ohfs_remove_field_PT_set( ohfs_remove_buffer_t *b_ptr,
                                             ohfs_remove_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_PT_set( ohfs_remove_buffer_t *b_ptr,
                                             ohfs_remove_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_PT_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_PT_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_PT_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 31:24) field PT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_PT_get( ohfs_remove_buffer_t *b_ptr,
                                               ohfs_remove_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_PT_get( ohfs_remove_buffer_t *b_ptr,
                                               ohfs_remove_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_PT_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 31:24) field PT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_PT_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_range_PT_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_range_PT_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_PT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_PT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_PT_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_PT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000400 + (N) * 0x8) bits 31:24) field PT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
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
        /* ((0x00000400 + (N) * 0x8) bits 31:24) field PT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
        ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_OFF + subfield_offset),
                                                    OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_remove_field_range_PT_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_PT_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_PT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_PT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_PT_get", stop_bit, 7 );
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
    /* ((0x00000400 + (N) * 0x8) bits 31:24) field PT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_MSK)
                  >> OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_MSK, OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_PT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_remove_field_MSI_ACCEPTANCE_CNT_set( ohfs_remove_buffer_t *b_ptr,
                                                             ohfs_remove_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_MSI_ACCEPTANCE_CNT_set( ohfs_remove_buffer_t *b_ptr,
                                                             ohfs_remove_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MSI_ACCEPTANCE_CNT_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_MSI_ACCEPTANCE_CNT_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_MSI_ACCEPTANCE_CNT_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 21:20) field MSI_ACCEPTANCE_CNT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_MSI_ACCEPTANCE_CNT_get( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_MSI_ACCEPTANCE_CNT_get( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MSI_ACCEPTANCE_CNT_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 21:20) field MSI_ACCEPTANCE_CNT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_MSI_ACCEPTANCE_CNT_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_set( ohfs_remove_buffer_t *b_ptr,
                                                                   ohfs_remove_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_set( ohfs_remove_buffer_t *b_ptr,
                                                                   ohfs_remove_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000400 + (N) * 0x8) bits 21:20) field MSI_ACCEPTANCE_CNT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
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
        /* ((0x00000400 + (N) * 0x8) bits 21:20) field MSI_ACCEPTANCE_CNT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
        ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_OFF + subfield_offset),
                                                    OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_get( ohfs_remove_buffer_t *b_ptr,
                                                                     ohfs_remove_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_get( ohfs_remove_buffer_t *b_ptr,
                                                                     ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_get", stop_bit, 1 );
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
    /* ((0x00000400 + (N) * 0x8) bits 21:20) field MSI_ACCEPTANCE_CNT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_MSK)
                  >> OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_MSK, OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_remove_field_PT_ACCEPTANCE_CNT_set( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_PT_ACCEPTANCE_CNT_set( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_PT_ACCEPTANCE_CNT_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_PT_ACCEPTANCE_CNT_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_PT_ACCEPTANCE_CNT_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 17:16) field PT_ACCEPTANCE_CNT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_PT_ACCEPTANCE_CNT_get( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_PT_ACCEPTANCE_CNT_get( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_PT_ACCEPTANCE_CNT_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 17:16) field PT_ACCEPTANCE_CNT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_PT_ACCEPTANCE_CNT_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_range_PT_ACCEPTANCE_CNT_set( ohfs_remove_buffer_t *b_ptr,
                                                                  ohfs_remove_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_range_PT_ACCEPTANCE_CNT_set( ohfs_remove_buffer_t *b_ptr,
                                                                  ohfs_remove_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_PT_ACCEPTANCE_CNT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_PT_ACCEPTANCE_CNT_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_PT_ACCEPTANCE_CNT_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_PT_ACCEPTANCE_CNT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000400 + (N) * 0x8) bits 17:16) field PT_ACCEPTANCE_CNT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
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
        /* ((0x00000400 + (N) * 0x8) bits 17:16) field PT_ACCEPTANCE_CNT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
        ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_OFF + subfield_offset),
                                                    OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_remove_field_range_PT_ACCEPTANCE_CNT_get( ohfs_remove_buffer_t *b_ptr,
                                                                    ohfs_remove_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_PT_ACCEPTANCE_CNT_get( ohfs_remove_buffer_t *b_ptr,
                                                                    ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_PT_ACCEPTANCE_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_PT_ACCEPTANCE_CNT_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_PT_ACCEPTANCE_CNT_get", stop_bit, 1 );
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
    /* ((0x00000400 + (N) * 0x8) bits 17:16) field PT_ACCEPTANCE_CNT of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_MSK)
                  >> OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_MSK, OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_PT_ACCEPTANCE_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_remove_field_TS_NUM_TYPE_set( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_TS_NUM_TYPE_set( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_TS_NUM_TYPE_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_TS_NUM_TYPE_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_TS_NUM_TYPE_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 14:12) field TS_NUM_TYPE of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_TS_NUM_TYPE_get( ohfs_remove_buffer_t *b_ptr,
                                                        ohfs_remove_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_TS_NUM_TYPE_get( ohfs_remove_buffer_t *b_ptr,
                                                        ohfs_remove_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_TS_NUM_TYPE_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 14:12) field TS_NUM_TYPE of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_TS_NUM_TYPE_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_range_TS_NUM_TYPE_set( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_range_TS_NUM_TYPE_set( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_TS_NUM_TYPE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_TS_NUM_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_TS_NUM_TYPE_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_TS_NUM_TYPE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000400 + (N) * 0x8) bits 14:12) field TS_NUM_TYPE of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
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
        /* ((0x00000400 + (N) * 0x8) bits 14:12) field TS_NUM_TYPE of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
        ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_OFF + subfield_offset),
                                                    OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_remove_field_range_TS_NUM_TYPE_get( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_TS_NUM_TYPE_get( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_TS_NUM_TYPE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_TS_NUM_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_TS_NUM_TYPE_get", stop_bit, 2 );
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
    /* ((0x00000400 + (N) * 0x8) bits 14:12) field TS_NUM_TYPE of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_MSK)
                  >> OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_MSK, OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_TS_NUM_TYPE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_remove_field_MAP_DLOOMFI_TO_SSF_set( ohfs_remove_buffer_t *b_ptr,
                                                             ohfs_remove_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_MAP_DLOOMFI_TO_SSF_set( ohfs_remove_buffer_t *b_ptr,
                                                             ohfs_remove_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MAP_DLOOMFI_TO_SSF_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_MAP_DLOOMFI_TO_SSF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_MAP_DLOOMFI_TO_SSF_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 10) field MAP_DLOOMFI_TO_SSF of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DLOOMFI_TO_SSF_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DLOOMFI_TO_SSF_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_MAP_DLOOMFI_TO_SSF_get( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_MAP_DLOOMFI_TO_SSF_get( ohfs_remove_buffer_t *b_ptr,
                                                               ohfs_remove_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MAP_DLOOMFI_TO_SSF_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 10) field MAP_DLOOMFI_TO_SSF of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DLOOMFI_TO_SSF_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DLOOMFI_TO_SSF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_MAP_DLOOMFI_TO_SSF_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_HO_RCOH_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_HO_RCOH_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_HO_RCOH_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_HO_RCOH_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_HO_RCOH_EN_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 9) field HO_RCOH_EN of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_HO_RCOH_EN_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_HO_RCOH_EN_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_HO_RCOH_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_HO_RCOH_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_HO_RCOH_EN_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 9) field HO_RCOH_EN of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_HO_RCOH_EN_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_HO_RCOH_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_HO_RCOH_EN_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_FLEX_RCOH_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_FLEX_RCOH_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FLEX_RCOH_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_FLEX_RCOH_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_FLEX_RCOH_EN_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 8) field FLEX_RCOH_EN of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_FLEX_RCOH_EN_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_FLEX_RCOH_EN_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_FLEX_RCOH_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_FLEX_RCOH_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FLEX_RCOH_EN_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 8) field FLEX_RCOH_EN of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_FLEX_RCOH_EN_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_FLEX_RCOH_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_FLEX_RCOH_EN_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_MAP_DCSF_TO_SSF_set( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_MAP_DCSF_TO_SSF_set( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MAP_DCSF_TO_SSF_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_MAP_DCSF_TO_SSF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_MAP_DCSF_TO_SSF_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 7) field MAP_DCSF_TO_SSF of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DCSF_TO_SSF_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DCSF_TO_SSF_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_MAP_DCSF_TO_SSF_get( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_MAP_DCSF_TO_SSF_get( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MAP_DCSF_TO_SSF_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 7) field MAP_DCSF_TO_SSF of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DCSF_TO_SSF_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DCSF_TO_SSF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_MAP_DCSF_TO_SSF_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_MAP_DPLM_TO_SSF_set( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_MAP_DPLM_TO_SSF_set( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MAP_DPLM_TO_SSF_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_MAP_DPLM_TO_SSF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_MAP_DPLM_TO_SSF_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 6) field MAP_DPLM_TO_SSF of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DPLM_TO_SSF_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DPLM_TO_SSF_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_MAP_DPLM_TO_SSF_get( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_MAP_DPLM_TO_SSF_get( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MAP_DPLM_TO_SSF_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 6) field MAP_DPLM_TO_SSF of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DPLM_TO_SSF_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DPLM_TO_SSF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_MAP_DPLM_TO_SSF_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_MAP_SF_TO_SSF_set( ohfs_remove_buffer_t *b_ptr,
                                                        ohfs_remove_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_MAP_SF_TO_SSF_set( ohfs_remove_buffer_t *b_ptr,
                                                        ohfs_remove_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MAP_SF_TO_SSF_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_MAP_SF_TO_SSF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_MAP_SF_TO_SSF_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 5) field MAP_SF_TO_SSF of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_SF_TO_SSF_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_SF_TO_SSF_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_MAP_SF_TO_SSF_get( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_MAP_SF_TO_SSF_get( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MAP_SF_TO_SSF_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 5) field MAP_SF_TO_SSF of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_SF_TO_SSF_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_SF_TO_SSF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_MAP_SF_TO_SSF_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_CSF_CHK_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_CSF_CHK_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_CSF_CHK_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_CSF_CHK_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_CSF_CHK_EN_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 4) field CSF_CHK_EN of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_CSF_CHK_EN_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_CSF_CHK_EN_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_CSF_CHK_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_CSF_CHK_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_CSF_CHK_EN_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 4) field CSF_CHK_EN of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_CSF_CHK_EN_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_CSF_CHK_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_CSF_CHK_EN_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_MSI_CHK_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_MSI_CHK_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MSI_CHK_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_MSI_CHK_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_MSI_CHK_EN_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 3) field MSI_CHK_EN of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_CHK_EN_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_CHK_EN_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_MSI_CHK_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_MSI_CHK_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MSI_CHK_EN_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 3) field MSI_CHK_EN of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_CHK_EN_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_CHK_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_MSI_CHK_EN_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_PT_CHK_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                    ohfs_remove_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_PT_CHK_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                    ohfs_remove_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_PT_CHK_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_PT_CHK_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_PT_CHK_EN_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 2) field PT_CHK_EN of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_CHK_EN_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_CHK_EN_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_PT_CHK_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_PT_CHK_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_PT_CHK_EN_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 2) field PT_CHK_EN of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_CHK_EN_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_CHK_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_PT_CHK_EN_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_OPUKHDR_DATA_set( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_OPUKHDR_DATA_set( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_OPUKHDR_DATA_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_OPUKHDR_DATA_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_OPUKHDR_DATA_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 1) field OPUKHDR_DATA of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_OPUKHDR_DATA_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_OPUKHDR_DATA_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_OPUKHDR_DATA_get( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_OPUKHDR_DATA_get( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_OPUKHDR_DATA_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 1) field OPUKHDR_DATA of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_OPUKHDR_DATA_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_OPUKHDR_DATA_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_OPUKHDR_DATA_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_NULL_FRAME_MODE_set( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_NULL_FRAME_MODE_set( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_NULL_FRAME_MODE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_NULL_FRAME_MODE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_NULL_FRAME_MODE_set", N, value );

    /* ((0x00000400 + (N) * 0x8) bits 0) field NULL_FRAME_MODE of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    ohfs_remove_reg_COMMON_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_NULL_FRAME_MODE_MSK,
                                                OHFS_REMOVE_REG_COMMON_CFG_BIT_NULL_FRAME_MODE_OFF,
                                                value);
}

static INLINE UINT32 ohfs_remove_field_NULL_FRAME_MODE_get( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_NULL_FRAME_MODE_get( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_NULL_FRAME_MODE_get", N, 95);
    /* ((0x00000400 + (N) * 0x8) bits 0) field NULL_FRAME_MODE of register PMC_OHFS_REMOVE_REG_COMMON_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_COMMON_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_REMOVE_REG_COMMON_CFG_BIT_NULL_FRAME_MODE_MSK) >> OHFS_REMOVE_REG_COMMON_CFG_BIT_NULL_FRAME_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_NULL_FRAME_MODE_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_FS2_ADDR_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_FS2_ADDR_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS2_ADDR_set", N, 95);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_FS2_ADDR_set", value, 4095);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_FS2_ADDR_set", N, value );

    /* ((0x00000404 + (N) * 0x8) bits 31:20) field FS2_ADDR of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set( b_ptr,
                                                                        h_ptr,
                                                                        N,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_MSK,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_OFF,
                                                                        value);
}

static INLINE UINT32 ohfs_remove_field_FS2_ADDR_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_FS2_ADDR_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS2_ADDR_get", N, 95);
    /* ((0x00000404 + (N) * 0x8) bits 31:20) field FS2_ADDR of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    reg_value = ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read( b_ptr,
                                                                               h_ptr,
                                                                               N);
    value = (reg_value & OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_MSK) >> OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_FS2_ADDR_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_range_FS2_ADDR_set( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_range_FS2_ADDR_set( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_FS2_ADDR_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_FS2_ADDR_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_FS2_ADDR_set", stop_bit, 11 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_FS2_ADDR_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000404 + (N) * 0x8) bits 31:20) field FS2_ADDR of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
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
        /* ((0x00000404 + (N) * 0x8) bits 31:20) field FS2_ADDR of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
        ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set( b_ptr,
                                                                            h_ptr,
                                                                            N,
                                                                            subfield_mask << (OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_OFF + subfield_offset),
                                                                            OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_OFF + subfield_offset,
                                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_remove_field_range_FS2_ADDR_get( ohfs_remove_buffer_t *b_ptr,
                                                           ohfs_remove_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_FS2_ADDR_get( ohfs_remove_buffer_t *b_ptr,
                                                           ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_FS2_ADDR_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_FS2_ADDR_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_FS2_ADDR_get", stop_bit, 11 );
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
    /* ((0x00000404 + (N) * 0x8) bits 31:20) field FS2_ADDR of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    reg_value = ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read( b_ptr,
                                                                               h_ptr,
                                                                               N);
    field_value = (reg_value & OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_MSK)
                  >> OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_MSK, OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_FS2_ADDR_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_remove_field_FS2_16_8_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_FS2_16_8_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS2_16_8_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_FS2_16_8_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_FS2_16_8_set", N, value );

    /* ((0x00000404 + (N) * 0x8) bits 17) field FS2_16_8 of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set( b_ptr,
                                                                        h_ptr,
                                                                        N,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_16_8_MSK,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_16_8_OFF,
                                                                        value);
}

static INLINE UINT32 ohfs_remove_field_FS2_16_8_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_FS2_16_8_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS2_16_8_get", N, 95);
    /* ((0x00000404 + (N) * 0x8) bits 17) field FS2_16_8 of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    reg_value = ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read( b_ptr,
                                                                               h_ptr,
                                                                               N);
    value = (reg_value & OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_16_8_MSK) >> OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_16_8_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_FS2_16_8_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_FS2_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                 ohfs_remove_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_FS2_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                 ohfs_remove_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS2_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_FS2_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_FS2_EN_set", N, value );

    /* ((0x00000404 + (N) * 0x8) bits 16) field FS2_EN of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set( b_ptr,
                                                                        h_ptr,
                                                                        N,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_EN_MSK,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_EN_OFF,
                                                                        value);
}

static INLINE UINT32 ohfs_remove_field_FS2_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_FS2_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS2_EN_get", N, 95);
    /* ((0x00000404 + (N) * 0x8) bits 16) field FS2_EN of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    reg_value = ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read( b_ptr,
                                                                               h_ptr,
                                                                               N);
    value = (reg_value & OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_EN_MSK) >> OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_FS2_EN_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_FS1_ADDR_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_FS1_ADDR_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS1_ADDR_set", N, 95);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_FS1_ADDR_set", value, 4095);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_FS1_ADDR_set", N, value );

    /* ((0x00000404 + (N) * 0x8) bits 15:4) field FS1_ADDR of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set( b_ptr,
                                                                        h_ptr,
                                                                        N,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_MSK,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_OFF,
                                                                        value);
}

static INLINE UINT32 ohfs_remove_field_FS1_ADDR_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_FS1_ADDR_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS1_ADDR_get", N, 95);
    /* ((0x00000404 + (N) * 0x8) bits 15:4) field FS1_ADDR of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    reg_value = ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read( b_ptr,
                                                                               h_ptr,
                                                                               N);
    value = (reg_value & OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_MSK) >> OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_FS1_ADDR_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_range_FS1_ADDR_set( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_range_FS1_ADDR_set( ohfs_remove_buffer_t *b_ptr,
                                                         ohfs_remove_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_FS1_ADDR_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_FS1_ADDR_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_FS1_ADDR_set", stop_bit, 11 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_FS1_ADDR_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000404 + (N) * 0x8) bits 15:4) field FS1_ADDR of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
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
        /* ((0x00000404 + (N) * 0x8) bits 15:4) field FS1_ADDR of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
        ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set( b_ptr,
                                                                            h_ptr,
                                                                            N,
                                                                            subfield_mask << (OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_OFF + subfield_offset),
                                                                            OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_OFF + subfield_offset,
                                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_remove_field_range_FS1_ADDR_get( ohfs_remove_buffer_t *b_ptr,
                                                           ohfs_remove_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_FS1_ADDR_get( ohfs_remove_buffer_t *b_ptr,
                                                           ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_FS1_ADDR_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_FS1_ADDR_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_FS1_ADDR_get", stop_bit, 11 );
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
    /* ((0x00000404 + (N) * 0x8) bits 15:4) field FS1_ADDR of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    reg_value = ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read( b_ptr,
                                                                               h_ptr,
                                                                               N);
    field_value = (reg_value & OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_MSK)
                  >> OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_MSK, OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_FS1_ADDR_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_remove_field_FS1_16_8_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_FS1_16_8_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS1_16_8_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_FS1_16_8_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_FS1_16_8_set", N, value );

    /* ((0x00000404 + (N) * 0x8) bits 1) field FS1_16_8 of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set( b_ptr,
                                                                        h_ptr,
                                                                        N,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_16_8_MSK,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_16_8_OFF,
                                                                        value);
}

static INLINE UINT32 ohfs_remove_field_FS1_16_8_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_FS1_16_8_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS1_16_8_get", N, 95);
    /* ((0x00000404 + (N) * 0x8) bits 1) field FS1_16_8 of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    reg_value = ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read( b_ptr,
                                                                               h_ptr,
                                                                               N);
    value = (reg_value & OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_16_8_MSK) >> OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_16_8_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_FS1_16_8_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_FS1_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                 ohfs_remove_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_FS1_EN_set( ohfs_remove_buffer_t *b_ptr,
                                                 ohfs_remove_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS1_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_FS1_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_FS1_EN_set", N, value );

    /* ((0x00000404 + (N) * 0x8) bits 0) field FS1_EN of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set( b_ptr,
                                                                        h_ptr,
                                                                        N,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_EN_MSK,
                                                                        OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_EN_OFF,
                                                                        value);
}

static INLINE UINT32 ohfs_remove_field_FS1_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_FS1_EN_get( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FS1_EN_get", N, 95);
    /* ((0x00000404 + (N) * 0x8) bits 0) field FS1_EN of register PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION index N=0..95 */
    reg_value = ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read( b_ptr,
                                                                               h_ptr,
                                                                               N);
    value = (reg_value & OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_EN_MSK) >> OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_FS1_EN_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_CHAN_MAP_TYPE_set( ohfs_remove_buffer_t *b_ptr,
                                                        ohfs_remove_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_CHAN_MAP_TYPE_set( ohfs_remove_buffer_t *b_ptr,
                                                        ohfs_remove_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_CHAN_MAP_TYPE_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_CHAN_MAP_TYPE_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_CHAN_MAP_TYPE_set", N, value );

    /* ((0x00000804 + (N) * 0x10) bits 10:8) field CHAN_MAP_TYPE of register PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG index N=0..95 */
    ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_MSK,
                                                         OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_OFF,
                                                         value);
}

static INLINE UINT32 ohfs_remove_field_CHAN_MAP_TYPE_get( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_CHAN_MAP_TYPE_get( ohfs_remove_buffer_t *b_ptr,
                                                          ohfs_remove_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_CHAN_MAP_TYPE_get", N, 95);
    /* ((0x00000804 + (N) * 0x10) bits 10:8) field CHAN_MAP_TYPE of register PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_read( b_ptr,
                                                                h_ptr,
                                                                N);
    value = (reg_value & OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_MSK) >> OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_CHAN_MAP_TYPE_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_range_CHAN_MAP_TYPE_set( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_range_CHAN_MAP_TYPE_set( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_CHAN_MAP_TYPE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_CHAN_MAP_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_CHAN_MAP_TYPE_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_CHAN_MAP_TYPE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000804 + (N) * 0x10) bits 10:8) field CHAN_MAP_TYPE of register PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG index N=0..95 */
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
        /* ((0x00000804 + (N) * 0x10) bits 10:8) field CHAN_MAP_TYPE of register PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG index N=0..95 */
        ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             N,
                                                             subfield_mask << (OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_OFF + subfield_offset),
                                                             OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_remove_field_range_CHAN_MAP_TYPE_get( ohfs_remove_buffer_t *b_ptr,
                                                                ohfs_remove_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_CHAN_MAP_TYPE_get( ohfs_remove_buffer_t *b_ptr,
                                                                ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_CHAN_MAP_TYPE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_CHAN_MAP_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_CHAN_MAP_TYPE_get", stop_bit, 2 );
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
    /* ((0x00000804 + (N) * 0x10) bits 10:8) field CHAN_MAP_TYPE of register PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_read( b_ptr,
                                                                h_ptr,
                                                                N);
    field_value = (reg_value & OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_MSK)
                  >> OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_MSK, OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_CHAN_MAP_TYPE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_remove_field_LAST_TS_set( ohfs_remove_buffer_t *b_ptr,
                                                  ohfs_remove_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_LAST_TS_set( ohfs_remove_buffer_t *b_ptr,
                                                  ohfs_remove_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_LAST_TS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_LAST_TS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_LAST_TS_set", N, value );

    /* ((0x00000804 + (N) * 0x10) bits 2) field LAST_TS of register PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG index N=0..95 */
    ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_LAST_TS_MSK,
                                                         OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_LAST_TS_OFF,
                                                         value);
}

static INLINE UINT32 ohfs_remove_field_LAST_TS_get( ohfs_remove_buffer_t *b_ptr,
                                                    ohfs_remove_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_LAST_TS_get( ohfs_remove_buffer_t *b_ptr,
                                                    ohfs_remove_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_LAST_TS_get", N, 95);
    /* ((0x00000804 + (N) * 0x10) bits 2) field LAST_TS of register PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_read( b_ptr,
                                                                h_ptr,
                                                                N);
    value = (reg_value & OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_LAST_TS_MSK) >> OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_LAST_TS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_LAST_TS_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_FIRST_TS_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_FIRST_TS_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FIRST_TS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_FIRST_TS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_FIRST_TS_set", N, value );

    /* ((0x00000804 + (N) * 0x10) bits 1) field FIRST_TS of register PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG index N=0..95 */
    ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_FIRST_TS_MSK,
                                                         OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_FIRST_TS_OFF,
                                                         value);
}

static INLINE UINT32 ohfs_remove_field_FIRST_TS_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_FIRST_TS_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_FIRST_TS_get", N, 95);
    /* ((0x00000804 + (N) * 0x10) bits 1) field FIRST_TS of register PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_read( b_ptr,
                                                                h_ptr,
                                                                N);
    value = (reg_value & OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_FIRST_TS_MSK) >> OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_FIRST_TS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_FIRST_TS_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_TS_VALID_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_TS_VALID_set( ohfs_remove_buffer_t *b_ptr,
                                                   ohfs_remove_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_TS_VALID_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_TS_VALID_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_TS_VALID_set", N, value );

    /* ((0x00000804 + (N) * 0x10) bits 0) field TS_VALID of register PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG index N=0..95 */
    ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_TS_VALID_MSK,
                                                         OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_TS_VALID_OFF,
                                                         value);
}

static INLINE UINT32 ohfs_remove_field_TS_VALID_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_TS_VALID_get( ohfs_remove_buffer_t *b_ptr,
                                                     ohfs_remove_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_TS_VALID_get", N, 95);
    /* ((0x00000804 + (N) * 0x10) bits 0) field TS_VALID of register PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_read( b_ptr,
                                                                h_ptr,
                                                                N);
    value = (reg_value & OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_TS_VALID_MSK) >> OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_TS_VALID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_TS_VALID_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_LO_ID_set( ohfs_remove_buffer_t *b_ptr,
                                                ohfs_remove_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_LO_ID_set( ohfs_remove_buffer_t *b_ptr,
                                                ohfs_remove_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_LO_ID_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_LO_ID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_LO_ID_set", N, value );

    /* ((0x00000808 + (N) * 0x10) bits 14:8) field LO_ID of register PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG index N=0..95 */
    ohfs_remove_reg_LO_ID_MSI_CFG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_MSK,
                                                   OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_OFF,
                                                   value);
}

static INLINE UINT32 ohfs_remove_field_LO_ID_get( ohfs_remove_buffer_t *b_ptr,
                                                  ohfs_remove_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_LO_ID_get( ohfs_remove_buffer_t *b_ptr,
                                                  ohfs_remove_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_LO_ID_get", N, 95);
    /* ((0x00000808 + (N) * 0x10) bits 14:8) field LO_ID of register PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_LO_ID_MSI_CFG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_MSK) >> OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_LO_ID_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_range_LO_ID_set( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_range_LO_ID_set( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_LO_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_LO_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_LO_ID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_LO_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000808 + (N) * 0x10) bits 14:8) field LO_ID of register PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG index N=0..95 */
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
        /* ((0x00000808 + (N) * 0x10) bits 14:8) field LO_ID of register PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG index N=0..95 */
        ohfs_remove_reg_LO_ID_MSI_CFG_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_OFF + subfield_offset),
                                                       OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_remove_field_range_LO_ID_get( ohfs_remove_buffer_t *b_ptr,
                                                        ohfs_remove_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_LO_ID_get( ohfs_remove_buffer_t *b_ptr,
                                                        ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_LO_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_LO_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_LO_ID_get", stop_bit, 6 );
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
    /* ((0x00000808 + (N) * 0x10) bits 14:8) field LO_ID of register PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_LO_ID_MSI_CFG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_MSK)
                  >> OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_MSK, OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_LO_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_remove_field_MSI_set( ohfs_remove_buffer_t *b_ptr,
                                              ohfs_remove_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_MSI_set( ohfs_remove_buffer_t *b_ptr,
                                              ohfs_remove_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MSI_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_remove_field_MSI_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_remove_field_MSI_set", N, value );

    /* ((0x00000808 + (N) * 0x10) bits 7:0) field MSI of register PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG index N=0..95 */
    ohfs_remove_reg_LO_ID_MSI_CFG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_MSK,
                                                   OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_OFF,
                                                   value);
}

static INLINE UINT32 ohfs_remove_field_MSI_get( ohfs_remove_buffer_t *b_ptr,
                                                ohfs_remove_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_MSI_get( ohfs_remove_buffer_t *b_ptr,
                                                ohfs_remove_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MSI_get", N, 95);
    /* ((0x00000808 + (N) * 0x10) bits 7:0) field MSI of register PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_LO_ID_MSI_CFG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_MSK) >> OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_MSI_get", N, value );

    return value;
}
static INLINE void ohfs_remove_field_range_MSI_set( ohfs_remove_buffer_t *b_ptr,
                                                    ohfs_remove_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_remove_field_range_MSI_set( ohfs_remove_buffer_t *b_ptr,
                                                    ohfs_remove_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_MSI_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_MSI_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_MSI_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_MSI_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000808 + (N) * 0x10) bits 7:0) field MSI of register PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG index N=0..95 */
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
        /* ((0x00000808 + (N) * 0x10) bits 7:0) field MSI of register PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG index N=0..95 */
        ohfs_remove_reg_LO_ID_MSI_CFG_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_OFF + subfield_offset),
                                                       OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_remove_field_range_MSI_get( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_MSI_get( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_MSI_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_MSI_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_MSI_get", stop_bit, 7 );
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
    /* ((0x00000808 + (N) * 0x10) bits 7:0) field MSI of register PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG index N=0..95 */
    reg_value = ohfs_remove_reg_LO_ID_MSI_CFG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_MSK)
                  >> OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_MSK, OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_MSI_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 ohfs_remove_field_PT_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_PT_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                      ohfs_remove_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_PT_STATUS_get", N, 95);
    /* ((0x00001000 + (N) * 0x4) bits 7:0) field PT_STATUS of register PMC_OHFS_REMOVE_REG_PT_STAT_REG index N=0..95 */
    reg_value = ohfs_remove_reg_PT_STAT_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & OHFS_REMOVE_REG_PT_STAT_REG_BIT_PT_STATUS_MSK) >> OHFS_REMOVE_REG_PT_STAT_REG_BIT_PT_STATUS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_PT_STATUS_get", N, value );

    return value;
}
static INLINE UINT32 ohfs_remove_field_range_PT_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_PT_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                            ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_PT_STATUS_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_PT_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_PT_STATUS_get", stop_bit, 7 );
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
    /* ((0x00001000 + (N) * 0x4) bits 7:0) field PT_STATUS of register PMC_OHFS_REMOVE_REG_PT_STAT_REG index N=0..95 */
    reg_value = ohfs_remove_reg_PT_STAT_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & OHFS_REMOVE_REG_PT_STAT_REG_BIT_PT_STATUS_MSK)
                  >> OHFS_REMOVE_REG_PT_STAT_REG_BIT_PT_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_PT_STAT_REG_BIT_PT_STATUS_MSK, OHFS_REMOVE_REG_PT_STAT_REG_BIT_PT_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_PT_STATUS_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 ohfs_remove_field_MSI_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_MSI_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                       ohfs_remove_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_MSI_STATUS_get", N, 95);
    /* ((0x00001200 + (N) * 0x4) bits 7:0) field MSI_STATUS of register PMC_OHFS_REMOVE_REG_MSI_STAT_REG index N=0..95 */
    reg_value = ohfs_remove_reg_MSI_STAT_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OHFS_REMOVE_REG_MSI_STAT_REG_BIT_MSI_STATUS_MSK) >> OHFS_REMOVE_REG_MSI_STAT_REG_BIT_MSI_STATUS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_MSI_STATUS_get", N, value );

    return value;
}
static INLINE UINT32 ohfs_remove_field_range_MSI_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                             ohfs_remove_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_MSI_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                             ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_MSI_STATUS_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_MSI_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_MSI_STATUS_get", stop_bit, 7 );
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
    /* ((0x00001200 + (N) * 0x4) bits 7:0) field MSI_STATUS of register PMC_OHFS_REMOVE_REG_MSI_STAT_REG index N=0..95 */
    reg_value = ohfs_remove_reg_MSI_STAT_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & OHFS_REMOVE_REG_MSI_STAT_REG_BIT_MSI_STATUS_MSK)
                  >> OHFS_REMOVE_REG_MSI_STAT_REG_BIT_MSI_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_MSI_STAT_REG_BIT_MSI_STATUS_MSK, OHFS_REMOVE_REG_MSI_STAT_REG_BIT_MSI_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_MSI_STATUS_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 ohfs_remove_field_RCOH_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                        ohfs_remove_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_RCOH_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                        ohfs_remove_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_RCOH_STATUS_get", N, 95);
    /* ((0x00001400 + (N) * 0x4) bits 15:0) field RCOH_STATUS of register PMC_OHFS_REMOVE_REG_RCOH_STAT_REG index N=0..95 */
    reg_value = ohfs_remove_reg_RCOH_STAT_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OHFS_REMOVE_REG_RCOH_STAT_REG_BIT_RCOH_STATUS_MSK) >> OHFS_REMOVE_REG_RCOH_STAT_REG_BIT_RCOH_STATUS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_remove_field_RCOH_STATUS_get", N, value );

    return value;
}
static INLINE UINT32 ohfs_remove_field_range_RCOH_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_remove_field_range_RCOH_STATUS_get( ohfs_remove_buffer_t *b_ptr,
                                                              ohfs_remove_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_remove_field_range_RCOH_STATUS_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_remove_field_range_RCOH_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_remove_field_range_RCOH_STATUS_get", stop_bit, 15 );
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
    /* ((0x00001400 + (N) * 0x4) bits 15:0) field RCOH_STATUS of register PMC_OHFS_REMOVE_REG_RCOH_STAT_REG index N=0..95 */
    reg_value = ohfs_remove_reg_RCOH_STAT_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & OHFS_REMOVE_REG_RCOH_STAT_REG_BIT_RCOH_STATUS_MSK)
                  >> OHFS_REMOVE_REG_RCOH_STAT_REG_BIT_RCOH_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_REMOVE_REG_RCOH_STAT_REG_BIT_RCOH_STATUS_MSK, OHFS_REMOVE_REG_RCOH_STAT_REG_BIT_RCOH_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_remove_field_range_RCOH_STATUS_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OHFS_REMOVE_IO_INLINE_H */
