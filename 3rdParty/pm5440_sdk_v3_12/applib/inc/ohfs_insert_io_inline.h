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
 *     and register accessor functions for the ohfs_insert block
 *****************************************************************************/
#ifndef _OHFS_INSERT_IO_INLINE_H
#define _OHFS_INSERT_IO_INLINE_H

#include "ohfs_insert_loc.h"
#include "ohfs_insert_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OHFS_INSERT_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for ohfs_insert
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
    ohfs_insert_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} ohfs_insert_buffer_t;
static INLINE void ohfs_insert_buffer_init( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void ohfs_insert_buffer_init( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "ohfs_insert_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void ohfs_insert_buffer_flush( ohfs_insert_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void ohfs_insert_buffer_flush( ohfs_insert_buffer_t *b_ptr )
{
    IOLOG( "ohfs_insert_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 ohfs_insert_reg_read( ohfs_insert_buffer_t *b_ptr,
                                           ohfs_insert_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_reg_read( ohfs_insert_buffer_t *b_ptr,
                                           ohfs_insert_handle_t *h_ptr,
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
static INLINE void ohfs_insert_reg_write( ohfs_insert_buffer_t *b_ptr,
                                          ohfs_insert_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_write( ohfs_insert_buffer_t *b_ptr,
                                          ohfs_insert_handle_t *h_ptr,
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

static INLINE void ohfs_insert_field_set( ohfs_insert_buffer_t *b_ptr,
                                          ohfs_insert_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 mask,
                                          UINT32 unused_mask,
                                          UINT32 ofs,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_set( ohfs_insert_buffer_t *b_ptr,
                                          ohfs_insert_handle_t *h_ptr,
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

static INLINE void ohfs_insert_action_on_write_field_set( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32 mem_type,
                                                          UINT32 reg,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_action_on_write_field_set( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
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

static INLINE void ohfs_insert_burst_read( ohfs_insert_buffer_t *b_ptr,
                                           ohfs_insert_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_burst_read( ohfs_insert_buffer_t *b_ptr,
                                           ohfs_insert_handle_t *h_ptr,
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

static INLINE void ohfs_insert_burst_write( ohfs_insert_buffer_t *b_ptr,
                                            ohfs_insert_handle_t *h_ptr,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_burst_write( ohfs_insert_buffer_t *b_ptr,
                                            ohfs_insert_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE ohfs_insert_poll( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 value,
                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                     UINT32 max_count,
                                                     UINT32 *num_failed_polls,
                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE ohfs_insert_poll( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
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
 *  register access functions for ohfs_insert
 * ==================================================================================
 */

static INLINE void ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                      ohfs_insert_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                      ohfs_insert_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_write", value );
    ohfs_insert_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG(N),
                           value);
}

static INLINE void ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                          ohfs_insert_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                          ohfs_insert_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_field_set", N, mask, ofs, value );
    ohfs_insert_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG(N),
                           mask,
                           PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                       ohfs_insert_handle_t *h_ptr,
                                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                       ohfs_insert_handle_t *h_ptr,
                                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_insert_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_insert_reg_JC_RD_IPT_ENTRY_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_JC_RD_IPT_ENTRY_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_insert_reg_JC_RD_IPT_ENTRY_array_write", value );
    ohfs_insert_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY(N),
                           value);
}

static INLINE void ohfs_insert_reg_JC_RD_IPT_ENTRY_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                    ohfs_insert_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_JC_RD_IPT_ENTRY_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                    ohfs_insert_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_insert_reg_JC_RD_IPT_ENTRY_array_field_set", N, mask, ofs, value );
    ohfs_insert_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY(N),
                           mask,
                           PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_insert_reg_JC_RD_IPT_ENTRY_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                 ohfs_insert_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_reg_JC_RD_IPT_ENTRY_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                 ohfs_insert_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_insert_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_insert_reg_JC_RD_IPT_ENTRY_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_insert_reg_JC_WR_IPT_ENTRY_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_JC_WR_IPT_ENTRY_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_insert_reg_JC_WR_IPT_ENTRY_array_write", value );
    ohfs_insert_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY(N),
                           value);
}

static INLINE void ohfs_insert_reg_JC_WR_IPT_ENTRY_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                    ohfs_insert_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_JC_WR_IPT_ENTRY_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                    ohfs_insert_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_insert_reg_JC_WR_IPT_ENTRY_array_field_set", N, mask, ofs, value );
    ohfs_insert_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY(N),
                           mask,
                           PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_insert_reg_JC_WR_IPT_ENTRY_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                 ohfs_insert_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_reg_JC_WR_IPT_ENTRY_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                 ohfs_insert_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_insert_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_insert_reg_JC_WR_IPT_ENTRY_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                          ohfs_insert_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                          ohfs_insert_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_write", value );
    ohfs_insert_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG(N),
                           value);
}

static INLINE void ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                              ohfs_insert_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                              ohfs_insert_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_field_set", N, mask, ofs, value );
    ohfs_insert_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_CONFIG,
                           PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG(N),
                           mask,
                           PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                           ohfs_insert_handle_t *h_ptr,
                                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                           ohfs_insert_handle_t *h_ptr,
                                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_insert_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_CONFIG,
                                      PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                      ohfs_insert_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                      ohfs_insert_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_write", value );
    ohfs_insert_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG(N),
                           value);
}

static INLINE void ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                          ohfs_insert_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                          ohfs_insert_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set", N, mask, ofs, value );
    ohfs_insert_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG(N),
                           mask,
                           PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                       ohfs_insert_handle_t *h_ptr,
                                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                       ohfs_insert_handle_t *h_ptr,
                                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_insert_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_insert_reg_FIXED_STUFF_CONFIG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                   ohfs_insert_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_FIXED_STUFF_CONFIG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                   ohfs_insert_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_insert_reg_FIXED_STUFF_CONFIG_array_write", value );
    ohfs_insert_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG(N),
                           value);
}

static INLINE void ohfs_insert_reg_FIXED_STUFF_CONFIG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                       ohfs_insert_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_FIXED_STUFF_CONFIG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                       ohfs_insert_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_insert_reg_FIXED_STUFF_CONFIG_array_field_set", N, mask, ofs, value );
    ohfs_insert_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG(N),
                           mask,
                           PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_insert_reg_FIXED_STUFF_CONFIG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                    ohfs_insert_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_reg_FIXED_STUFF_CONFIG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                    ohfs_insert_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_insert_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_insert_reg_FIXED_STUFF_CONFIG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                    ohfs_insert_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                                    ohfs_insert_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_write", value );
    ohfs_insert_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG(N),
                           value);
}

static INLINE void ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                        ohfs_insert_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                        ohfs_insert_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_field_set", N, mask, ofs, value );
    ohfs_insert_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG(N),
                           mask,
                           PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                     ohfs_insert_handle_t *h_ptr,
                                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                                     ohfs_insert_handle_t *h_ptr,
                                                                     UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_insert_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_insert_reg_XOFF_WM_CFG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_XOFF_WM_CFG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_insert_reg_XOFF_WM_CFG_array_write", value );
    ohfs_insert_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_INSERT_REG_XOFF_WM_CFG(N),
                           value);
}

static INLINE void ohfs_insert_reg_XOFF_WM_CFG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_XOFF_WM_CFG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_insert_reg_XOFF_WM_CFG_array_field_set", N, mask, ofs, value );
    ohfs_insert_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_INSERT_REG_XOFF_WM_CFG(N),
                           mask,
                           PMC_OHFS_INSERT_REG_XOFF_WM_CFG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_insert_reg_XOFF_WM_CFG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_reg_XOFF_WM_CFG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_insert_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_INSERT_REG_XOFF_WM_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_insert_reg_XOFF_WM_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void ohfs_insert_reg_LO_CFG_REG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                           ohfs_insert_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_LO_CFG_REG_array_write( ohfs_insert_buffer_t *b_ptr,
                                                           ohfs_insert_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "ohfs_insert_reg_LO_CFG_REG_array_write", value );
    ohfs_insert_reg_write( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_INSERT_REG_LO_CFG_REG(N),
                           value);
}

static INLINE void ohfs_insert_reg_LO_CFG_REG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_reg_LO_CFG_REG_array_field_set( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "ohfs_insert_reg_LO_CFG_REG_array_field_set", N, mask, ofs, value );
    ohfs_insert_field_set( b_ptr,
                           h_ptr,
                           MEM_TYPE_STATUS,
                           PMC_OHFS_INSERT_REG_LO_CFG_REG(N),
                           mask,
                           PMC_OHFS_INSERT_REG_LO_CFG_REG_UNUSED_MASK,
                           ofs,
                           value);

}

static INLINE UINT32 ohfs_insert_reg_LO_CFG_REG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_reg_LO_CFG_REG_array_read( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = ohfs_insert_reg_read( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_OHFS_INSERT_REG_LO_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "ohfs_insert_reg_LO_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void ohfs_insert_field_NCS_EN_set( ohfs_insert_buffer_t *b_ptr,
                                                 ohfs_insert_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_NCS_EN_set( ohfs_insert_buffer_t *b_ptr,
                                                 ohfs_insert_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_NCS_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_NCS_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_NCS_EN_set", N, value );

    /* ((0x00000020 + (N) * 0x04) bits 1) field NCS_EN of register PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG index N=0..95 */
    ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_NCS_EN_MSK,
                                                           OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_NCS_EN_OFF,
                                                           value);
}

static INLINE UINT32 ohfs_insert_field_NCS_EN_get( ohfs_insert_buffer_t *b_ptr,
                                                   ohfs_insert_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_NCS_EN_get( ohfs_insert_buffer_t *b_ptr,
                                                   ohfs_insert_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_NCS_EN_get", N, 95);
    /* ((0x00000020 + (N) * 0x04) bits 1) field NCS_EN of register PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_NCS_EN_MSK) >> OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_NCS_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_NCS_EN_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_RP_set( ohfs_insert_buffer_t *b_ptr,
                                             ohfs_insert_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_RP_set( ohfs_insert_buffer_t *b_ptr,
                                             ohfs_insert_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_RP_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_RP_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_RP_set", N, value );

    /* ((0x00000020 + (N) * 0x04) bits 0) field RP of register PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG index N=0..95 */
    ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_RP_MSK,
                                                           OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_RP_OFF,
                                                           value);
}

static INLINE UINT32 ohfs_insert_field_RP_get( ohfs_insert_buffer_t *b_ptr,
                                               ohfs_insert_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_RP_get( ohfs_insert_buffer_t *b_ptr,
                                               ohfs_insert_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_RP_get", N, 95);
    /* ((0x00000020 + (N) * 0x04) bits 0) field RP of register PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_RP_MSK) >> OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_RP_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_RP_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_JC_RD_IPT_VALID_set( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_JC_RD_IPT_VALID_set( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_RD_IPT_VALID_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_JC_RD_IPT_VALID_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_JC_RD_IPT_VALID_set", N, value );

    /* ((0x00000210 + (N) * 0x20) bits 16) field JC_RD_IPT_VALID of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
    ohfs_insert_reg_JC_RD_IPT_ENTRY_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VALID_MSK,
                                                     OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VALID_OFF,
                                                     value);
}

static INLINE UINT32 ohfs_insert_field_JC_RD_IPT_VALID_get( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_JC_RD_IPT_VALID_get( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_RD_IPT_VALID_get", N, 95);
    /* ((0x00000210 + (N) * 0x20) bits 16) field JC_RD_IPT_VALID of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
    reg_value = ohfs_insert_reg_JC_RD_IPT_ENTRY_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VALID_MSK) >> OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VALID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_JC_RD_IPT_VALID_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_JC_RD_IPT_ID_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_JC_RD_IPT_ID_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_RD_IPT_ID_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_JC_RD_IPT_ID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_JC_RD_IPT_ID_set", N, value );

    /* ((0x00000210 + (N) * 0x20) bits 14:8) field JC_RD_IPT_ID of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
    ohfs_insert_reg_JC_RD_IPT_ENTRY_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_MSK,
                                                     OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_OFF,
                                                     value);
}

static INLINE UINT32 ohfs_insert_field_JC_RD_IPT_ID_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_JC_RD_IPT_ID_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_RD_IPT_ID_get", N, 95);
    /* ((0x00000210 + (N) * 0x20) bits 14:8) field JC_RD_IPT_ID of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
    reg_value = ohfs_insert_reg_JC_RD_IPT_ENTRY_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_MSK) >> OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_JC_RD_IPT_ID_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_JC_RD_IPT_ID_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_JC_RD_IPT_ID_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_JC_RD_IPT_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_JC_RD_IPT_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_JC_RD_IPT_ID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_JC_RD_IPT_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000210 + (N) * 0x20) bits 14:8) field JC_RD_IPT_ID of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
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
        /* ((0x00000210 + (N) * 0x20) bits 14:8) field JC_RD_IPT_ID of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
        ohfs_insert_reg_JC_RD_IPT_ENTRY_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_OFF + subfield_offset),
                                                         OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_JC_RD_IPT_ID_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_JC_RD_IPT_ID_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_JC_RD_IPT_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_JC_RD_IPT_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_JC_RD_IPT_ID_get", stop_bit, 6 );
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
    /* ((0x00000210 + (N) * 0x20) bits 14:8) field JC_RD_IPT_ID of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
    reg_value = ohfs_insert_reg_JC_RD_IPT_ENTRY_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_MSK)
                  >> OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_MSK, OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_JC_RD_IPT_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_JC_RD_IPT_VA_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_JC_RD_IPT_VA_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_RD_IPT_VA_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_JC_RD_IPT_VA_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_JC_RD_IPT_VA_set", N, value );

    /* ((0x00000210 + (N) * 0x20) bits 6:0) field JC_RD_IPT_VA of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
    ohfs_insert_reg_JC_RD_IPT_ENTRY_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_MSK,
                                                     OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_OFF,
                                                     value);
}

static INLINE UINT32 ohfs_insert_field_JC_RD_IPT_VA_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_JC_RD_IPT_VA_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_RD_IPT_VA_get", N, 95);
    /* ((0x00000210 + (N) * 0x20) bits 6:0) field JC_RD_IPT_VA of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
    reg_value = ohfs_insert_reg_JC_RD_IPT_ENTRY_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_MSK) >> OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_JC_RD_IPT_VA_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_JC_RD_IPT_VA_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_JC_RD_IPT_VA_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_JC_RD_IPT_VA_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_JC_RD_IPT_VA_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_JC_RD_IPT_VA_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_JC_RD_IPT_VA_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000210 + (N) * 0x20) bits 6:0) field JC_RD_IPT_VA of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
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
        /* ((0x00000210 + (N) * 0x20) bits 6:0) field JC_RD_IPT_VA of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
        ohfs_insert_reg_JC_RD_IPT_ENTRY_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_OFF + subfield_offset),
                                                         OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_JC_RD_IPT_VA_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_JC_RD_IPT_VA_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_JC_RD_IPT_VA_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_JC_RD_IPT_VA_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_JC_RD_IPT_VA_get", stop_bit, 6 );
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
    /* ((0x00000210 + (N) * 0x20) bits 6:0) field JC_RD_IPT_VA of register PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY index N=0..95 */
    reg_value = ohfs_insert_reg_JC_RD_IPT_ENTRY_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_MSK)
                  >> OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_MSK, OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_JC_RD_IPT_VA_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_JC_WR_IPT_VALID_set( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_JC_WR_IPT_VALID_set( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_WR_IPT_VALID_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_JC_WR_IPT_VALID_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_JC_WR_IPT_VALID_set", N, value );

    /* ((0x00000214 + (N) * 0x20) bits 16) field JC_WR_IPT_VALID of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
    ohfs_insert_reg_JC_WR_IPT_ENTRY_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VALID_MSK,
                                                     OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VALID_OFF,
                                                     value);
}

static INLINE UINT32 ohfs_insert_field_JC_WR_IPT_VALID_get( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_JC_WR_IPT_VALID_get( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_WR_IPT_VALID_get", N, 95);
    /* ((0x00000214 + (N) * 0x20) bits 16) field JC_WR_IPT_VALID of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
    reg_value = ohfs_insert_reg_JC_WR_IPT_ENTRY_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VALID_MSK) >> OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VALID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_JC_WR_IPT_VALID_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_JC_WR_IPT_ID_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_JC_WR_IPT_ID_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_WR_IPT_ID_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_JC_WR_IPT_ID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_JC_WR_IPT_ID_set", N, value );

    /* ((0x00000214 + (N) * 0x20) bits 14:8) field JC_WR_IPT_ID of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
    ohfs_insert_reg_JC_WR_IPT_ENTRY_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_MSK,
                                                     OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_OFF,
                                                     value);
}

static INLINE UINT32 ohfs_insert_field_JC_WR_IPT_ID_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_JC_WR_IPT_ID_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_WR_IPT_ID_get", N, 95);
    /* ((0x00000214 + (N) * 0x20) bits 14:8) field JC_WR_IPT_ID of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
    reg_value = ohfs_insert_reg_JC_WR_IPT_ENTRY_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_MSK) >> OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_JC_WR_IPT_ID_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_JC_WR_IPT_ID_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_JC_WR_IPT_ID_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_JC_WR_IPT_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_JC_WR_IPT_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_JC_WR_IPT_ID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_JC_WR_IPT_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000214 + (N) * 0x20) bits 14:8) field JC_WR_IPT_ID of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
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
        /* ((0x00000214 + (N) * 0x20) bits 14:8) field JC_WR_IPT_ID of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
        ohfs_insert_reg_JC_WR_IPT_ENTRY_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_OFF + subfield_offset),
                                                         OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_JC_WR_IPT_ID_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_JC_WR_IPT_ID_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_JC_WR_IPT_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_JC_WR_IPT_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_JC_WR_IPT_ID_get", stop_bit, 6 );
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
    /* ((0x00000214 + (N) * 0x20) bits 14:8) field JC_WR_IPT_ID of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
    reg_value = ohfs_insert_reg_JC_WR_IPT_ENTRY_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_MSK)
                  >> OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_MSK, OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_JC_WR_IPT_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_JC_WR_IPT_VA_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_JC_WR_IPT_VA_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_WR_IPT_VA_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_JC_WR_IPT_VA_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_JC_WR_IPT_VA_set", N, value );

    /* ((0x00000214 + (N) * 0x20) bits 6:0) field JC_WR_IPT_VA of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
    ohfs_insert_reg_JC_WR_IPT_ENTRY_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_MSK,
                                                     OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_OFF,
                                                     value);
}

static INLINE UINT32 ohfs_insert_field_JC_WR_IPT_VA_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_JC_WR_IPT_VA_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_JC_WR_IPT_VA_get", N, 95);
    /* ((0x00000214 + (N) * 0x20) bits 6:0) field JC_WR_IPT_VA of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
    reg_value = ohfs_insert_reg_JC_WR_IPT_ENTRY_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_MSK) >> OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_JC_WR_IPT_VA_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_JC_WR_IPT_VA_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_JC_WR_IPT_VA_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_JC_WR_IPT_VA_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_JC_WR_IPT_VA_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_JC_WR_IPT_VA_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_JC_WR_IPT_VA_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000214 + (N) * 0x20) bits 6:0) field JC_WR_IPT_VA of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
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
        /* ((0x00000214 + (N) * 0x20) bits 6:0) field JC_WR_IPT_VA of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
        ohfs_insert_reg_JC_WR_IPT_ENTRY_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_OFF + subfield_offset),
                                                         OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_JC_WR_IPT_VA_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_JC_WR_IPT_VA_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_JC_WR_IPT_VA_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_JC_WR_IPT_VA_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_JC_WR_IPT_VA_get", stop_bit, 6 );
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
    /* ((0x00000214 + (N) * 0x20) bits 6:0) field JC_WR_IPT_VA of register PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY index N=0..95 */
    reg_value = ohfs_insert_reg_JC_WR_IPT_ENTRY_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_MSK)
                  >> OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_MSK, OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_JC_WR_IPT_VA_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_CSF_INSERT_set( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_CSF_INSERT_set( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_CSF_INSERT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_CSF_INSERT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_CSF_INSERT_set", N, value );

    /* ((0x00000218 + (N) * 0x20) bits 0) field CSF_INSERT of register PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG index N=0..95 */
    ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_BIT_CSF_INSERT_MSK,
                                                               OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_BIT_CSF_INSERT_OFF,
                                                               value);
}

static INLINE UINT32 ohfs_insert_field_CSF_INSERT_get( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_CSF_INSERT_get( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_CSF_INSERT_get", N, 95);
    /* ((0x00000218 + (N) * 0x20) bits 0) field CSF_INSERT of register PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_read( b_ptr,
                                                                      h_ptr,
                                                                      N);
    value = (reg_value & OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_BIT_CSF_INSERT_MSK) >> OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_BIT_CSF_INSERT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_CSF_INSERT_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void ohfs_insert_field_DBG_UNDERRUN_INT_SRC_set( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_DBG_UNDERRUN_INT_SRC_set( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_DBG_UNDERRUN_INT_SRC_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_DBG_UNDERRUN_INT_SRC_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_DBG_UNDERRUN_INT_SRC_set", N, value );

    /* ((0x00000200 + (N) * 0x20) bits 26) field DBG_UNDERRUN_INT_SRC of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_UNDERRUN_INT_SRC_MSK,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_UNDERRUN_INT_SRC_OFF,
                                                           value);
}

static INLINE UINT32 ohfs_insert_field_DBG_UNDERRUN_INT_SRC_get( ohfs_insert_buffer_t *b_ptr,
                                                                 ohfs_insert_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_DBG_UNDERRUN_INT_SRC_get( ohfs_insert_buffer_t *b_ptr,
                                                                 ohfs_insert_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_DBG_UNDERRUN_INT_SRC_get", N, 95);
    /* ((0x00000200 + (N) * 0x20) bits 26) field DBG_UNDERRUN_INT_SRC of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_UNDERRUN_INT_SRC_MSK) >> OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_UNDERRUN_INT_SRC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_DBG_UNDERRUN_INT_SRC_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_DBG_MASK_SOMF_SYNC_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_DBG_MASK_SOMF_SYNC_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_DBG_MASK_SOMF_SYNC_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_DBG_MASK_SOMF_SYNC_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_DBG_MASK_SOMF_SYNC_set", N, value );

    /* ((0x00000200 + (N) * 0x20) bits 25) field DBG_MASK_SOMF_SYNC of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_MASK_SOMF_SYNC_MSK,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_MASK_SOMF_SYNC_OFF,
                                                           value);
}

static INLINE UINT32 ohfs_insert_field_DBG_MASK_SOMF_SYNC_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_DBG_MASK_SOMF_SYNC_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_DBG_MASK_SOMF_SYNC_get", N, 95);
    /* ((0x00000200 + (N) * 0x20) bits 25) field DBG_MASK_SOMF_SYNC of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_MASK_SOMF_SYNC_MSK) >> OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_MASK_SOMF_SYNC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_DBG_MASK_SOMF_SYNC_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_EXT_PAYLOAD_TYPE_set( ohfs_insert_buffer_t *b_ptr,
                                                           ohfs_insert_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_EXT_PAYLOAD_TYPE_set( ohfs_insert_buffer_t *b_ptr,
                                                           ohfs_insert_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_EXT_PAYLOAD_TYPE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_EXT_PAYLOAD_TYPE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_EXT_PAYLOAD_TYPE_set", N, value );

    /* ((0x00000200 + (N) * 0x20) bits 24) field EXT_PAYLOAD_TYPE of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_EXT_PAYLOAD_TYPE_MSK,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_EXT_PAYLOAD_TYPE_OFF,
                                                           value);
}

static INLINE UINT32 ohfs_insert_field_EXT_PAYLOAD_TYPE_get( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_EXT_PAYLOAD_TYPE_get( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_EXT_PAYLOAD_TYPE_get", N, 95);
    /* ((0x00000200 + (N) * 0x20) bits 24) field EXT_PAYLOAD_TYPE of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_EXT_PAYLOAD_TYPE_MSK) >> OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_EXT_PAYLOAD_TYPE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_EXT_PAYLOAD_TYPE_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_PT_set( ohfs_insert_buffer_t *b_ptr,
                                             ohfs_insert_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_PT_set( ohfs_insert_buffer_t *b_ptr,
                                             ohfs_insert_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_PT_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_PT_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_PT_set", N, value );

    /* ((0x00000200 + (N) * 0x20) bits 23:16) field PT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_MSK,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_OFF,
                                                           value);
}

static INLINE UINT32 ohfs_insert_field_PT_get( ohfs_insert_buffer_t *b_ptr,
                                               ohfs_insert_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_PT_get( ohfs_insert_buffer_t *b_ptr,
                                               ohfs_insert_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_PT_get", N, 95);
    /* ((0x00000200 + (N) * 0x20) bits 23:16) field PT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_MSK) >> OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_PT_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_PT_set( ohfs_insert_buffer_t *b_ptr,
                                                   ohfs_insert_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_PT_set( ohfs_insert_buffer_t *b_ptr,
                                                   ohfs_insert_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_PT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_PT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_PT_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_PT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000200 + (N) * 0x20) bits 23:16) field PT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
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
        /* ((0x00000200 + (N) * 0x20) bits 23:16) field PT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
        ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               subfield_mask << (OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_OFF + subfield_offset),
                                                               OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_PT_get( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_PT_get( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_PT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_PT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_PT_get", stop_bit, 7 );
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
    /* ((0x00000200 + (N) * 0x20) bits 23:16) field PT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    field_value = (reg_value & OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_MSK)
                  >> OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_MSK, OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_PT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_PAD_CNT_set( ohfs_insert_buffer_t *b_ptr,
                                                  ohfs_insert_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_PAD_CNT_set( ohfs_insert_buffer_t *b_ptr,
                                                  ohfs_insert_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_PAD_CNT_set", N, 95);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_PAD_CNT_set", value, 63);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_PAD_CNT_set", N, value );

    /* ((0x00000200 + (N) * 0x20) bits 15:10) field PAD_CNT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_MSK,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_OFF,
                                                           value);
}

static INLINE UINT32 ohfs_insert_field_PAD_CNT_get( ohfs_insert_buffer_t *b_ptr,
                                                    ohfs_insert_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_PAD_CNT_get( ohfs_insert_buffer_t *b_ptr,
                                                    ohfs_insert_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_PAD_CNT_get", N, 95);
    /* ((0x00000200 + (N) * 0x20) bits 15:10) field PAD_CNT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_MSK) >> OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_PAD_CNT_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_PAD_CNT_set( ohfs_insert_buffer_t *b_ptr,
                                                        ohfs_insert_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_PAD_CNT_set( ohfs_insert_buffer_t *b_ptr,
                                                        ohfs_insert_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_PAD_CNT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_PAD_CNT_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_PAD_CNT_set", stop_bit, 5 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_PAD_CNT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000200 + (N) * 0x20) bits 15:10) field PAD_CNT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
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
        if (stop_bit < 5) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 5;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000200 + (N) * 0x20) bits 15:10) field PAD_CNT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
        ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               subfield_mask << (OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_OFF + subfield_offset),
                                                               OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_PAD_CNT_get( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_PAD_CNT_get( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_PAD_CNT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_PAD_CNT_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_PAD_CNT_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000200 + (N) * 0x20) bits 15:10) field PAD_CNT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    field_value = (reg_value & OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_MSK)
                  >> OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_MSK, OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_PAD_CNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_MAX_SLOT_NUM_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_MAX_SLOT_NUM_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_MAX_SLOT_NUM_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_MAX_SLOT_NUM_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_MAX_SLOT_NUM_set", N, value );

    /* ((0x00000200 + (N) * 0x20) bits 9:3) field MAX_SLOT_NUM of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_MSK,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_OFF,
                                                           value);
}

static INLINE UINT32 ohfs_insert_field_MAX_SLOT_NUM_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_MAX_SLOT_NUM_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_MAX_SLOT_NUM_get", N, 95);
    /* ((0x00000200 + (N) * 0x20) bits 9:3) field MAX_SLOT_NUM of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_MSK) >> OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_MAX_SLOT_NUM_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_MAX_SLOT_NUM_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_MAX_SLOT_NUM_set( ohfs_insert_buffer_t *b_ptr,
                                                             ohfs_insert_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_MAX_SLOT_NUM_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_MAX_SLOT_NUM_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_MAX_SLOT_NUM_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_MAX_SLOT_NUM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000200 + (N) * 0x20) bits 9:3) field MAX_SLOT_NUM of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
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
        /* ((0x00000200 + (N) * 0x20) bits 9:3) field MAX_SLOT_NUM of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
        ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               subfield_mask << (OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_OFF + subfield_offset),
                                                               OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_MAX_SLOT_NUM_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_MAX_SLOT_NUM_get( ohfs_insert_buffer_t *b_ptr,
                                                               ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_MAX_SLOT_NUM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_MAX_SLOT_NUM_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_MAX_SLOT_NUM_get", stop_bit, 6 );
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
    /* ((0x00000200 + (N) * 0x20) bits 9:3) field MAX_SLOT_NUM of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    field_value = (reg_value & OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_MSK)
                  >> OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_MSK, OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_MAX_SLOT_NUM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_OMFI_INSERT_set( ohfs_insert_buffer_t *b_ptr,
                                                      ohfs_insert_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_OMFI_INSERT_set( ohfs_insert_buffer_t *b_ptr,
                                                      ohfs_insert_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_OMFI_INSERT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_OMFI_INSERT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_OMFI_INSERT_set", N, value );

    /* ((0x00000200 + (N) * 0x20) bits 2) field OMFI_INSERT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_OMFI_INSERT_MSK,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_OMFI_INSERT_OFF,
                                                           value);
}

static INLINE UINT32 ohfs_insert_field_OMFI_INSERT_get( ohfs_insert_buffer_t *b_ptr,
                                                        ohfs_insert_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_OMFI_INSERT_get( ohfs_insert_buffer_t *b_ptr,
                                                        ohfs_insert_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_OMFI_INSERT_get", N, 95);
    /* ((0x00000200 + (N) * 0x20) bits 2) field OMFI_INSERT of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_OMFI_INSERT_MSK) >> OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_OMFI_INSERT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_OMFI_INSERT_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_NULL_CHAN_EN_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_NULL_CHAN_EN_set( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_NULL_CHAN_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_NULL_CHAN_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_NULL_CHAN_EN_set", N, value );

    /* ((0x00000200 + (N) * 0x20) bits 0) field NULL_CHAN_EN of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_NULL_CHAN_EN_MSK,
                                                           OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_NULL_CHAN_EN_OFF,
                                                           value);
}

static INLINE UINT32 ohfs_insert_field_NULL_CHAN_EN_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_NULL_CHAN_EN_get( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_NULL_CHAN_EN_get", N, 95);
    /* ((0x00000200 + (N) * 0x20) bits 0) field NULL_CHAN_EN of register PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_NULL_CHAN_EN_MSK) >> OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_NULL_CHAN_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_NULL_CHAN_EN_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_STUFF_COL_POS_2_set( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_STUFF_COL_POS_2_set( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_STUFF_COL_POS_2_set", N, 95);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_STUFF_COL_POS_2_set", value, 4095);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_STUFF_COL_POS_2_set", N, value );

    /* ((0x00000204 + (N) * 0x20) bits 26:15) field STUFF_COL_POS_2 of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    ohfs_insert_reg_FIXED_STUFF_CONFIG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_MSK,
                                                        OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_OFF,
                                                        value);
}

static INLINE UINT32 ohfs_insert_field_STUFF_COL_POS_2_get( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_STUFF_COL_POS_2_get( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_STUFF_COL_POS_2_get", N, 95);
    /* ((0x00000204 + (N) * 0x20) bits 26:15) field STUFF_COL_POS_2 of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_FIXED_STUFF_CONFIG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_MSK) >> OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_STUFF_COL_POS_2_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_STUFF_COL_POS_2_set( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_STUFF_COL_POS_2_set( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_STUFF_COL_POS_2_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_STUFF_COL_POS_2_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_STUFF_COL_POS_2_set", stop_bit, 11 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_STUFF_COL_POS_2_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000204 + (N) * 0x20) bits 26:15) field STUFF_COL_POS_2 of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
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
        /* ((0x00000204 + (N) * 0x20) bits 26:15) field STUFF_COL_POS_2 of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
        ohfs_insert_reg_FIXED_STUFF_CONFIG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            subfield_mask << (OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_OFF + subfield_offset),
                                                            OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_STUFF_COL_POS_2_get( ohfs_insert_buffer_t *b_ptr,
                                                                  ohfs_insert_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_STUFF_COL_POS_2_get( ohfs_insert_buffer_t *b_ptr,
                                                                  ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_STUFF_COL_POS_2_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_STUFF_COL_POS_2_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_STUFF_COL_POS_2_get", stop_bit, 11 );
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
    /* ((0x00000204 + (N) * 0x20) bits 26:15) field STUFF_COL_POS_2 of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_FIXED_STUFF_CONFIG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_MSK)
                  >> OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_MSK, OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_STUFF_COL_POS_2_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_STUFF_COL_POS_1_set( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_STUFF_COL_POS_1_set( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_STUFF_COL_POS_1_set", N, 95);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_STUFF_COL_POS_1_set", value, 4095);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_STUFF_COL_POS_1_set", N, value );

    /* ((0x00000204 + (N) * 0x20) bits 14:3) field STUFF_COL_POS_1 of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    ohfs_insert_reg_FIXED_STUFF_CONFIG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_MSK,
                                                        OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_OFF,
                                                        value);
}

static INLINE UINT32 ohfs_insert_field_STUFF_COL_POS_1_get( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_STUFF_COL_POS_1_get( ohfs_insert_buffer_t *b_ptr,
                                                            ohfs_insert_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_STUFF_COL_POS_1_get", N, 95);
    /* ((0x00000204 + (N) * 0x20) bits 14:3) field STUFF_COL_POS_1 of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_FIXED_STUFF_CONFIG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_MSK) >> OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_STUFF_COL_POS_1_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_STUFF_COL_POS_1_set( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_STUFF_COL_POS_1_set( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_STUFF_COL_POS_1_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_STUFF_COL_POS_1_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_STUFF_COL_POS_1_set", stop_bit, 11 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_STUFF_COL_POS_1_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000204 + (N) * 0x20) bits 14:3) field STUFF_COL_POS_1 of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
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
        /* ((0x00000204 + (N) * 0x20) bits 14:3) field STUFF_COL_POS_1 of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
        ohfs_insert_reg_FIXED_STUFF_CONFIG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            subfield_mask << (OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_OFF + subfield_offset),
                                                            OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_STUFF_COL_POS_1_get( ohfs_insert_buffer_t *b_ptr,
                                                                  ohfs_insert_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_STUFF_COL_POS_1_get( ohfs_insert_buffer_t *b_ptr,
                                                                  ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_STUFF_COL_POS_1_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_STUFF_COL_POS_1_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_STUFF_COL_POS_1_get", stop_bit, 11 );
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
    /* ((0x00000204 + (N) * 0x20) bits 14:3) field STUFF_COL_POS_1 of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_FIXED_STUFF_CONFIG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_MSK)
                  >> OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_MSK, OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_STUFF_COL_POS_1_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_STUFF_POS_SEL_set( ohfs_insert_buffer_t *b_ptr,
                                                        ohfs_insert_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_STUFF_POS_SEL_set( ohfs_insert_buffer_t *b_ptr,
                                                        ohfs_insert_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_STUFF_POS_SEL_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_STUFF_POS_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_STUFF_POS_SEL_set", N, value );

    /* ((0x00000204 + (N) * 0x20) bits 2) field STUFF_POS_SEL of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    ohfs_insert_reg_FIXED_STUFF_CONFIG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_POS_SEL_MSK,
                                                        OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_POS_SEL_OFF,
                                                        value);
}

static INLINE UINT32 ohfs_insert_field_STUFF_POS_SEL_get( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_STUFF_POS_SEL_get( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_STUFF_POS_SEL_get", N, 95);
    /* ((0x00000204 + (N) * 0x20) bits 2) field STUFF_POS_SEL of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_FIXED_STUFF_CONFIG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_POS_SEL_MSK) >> OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_POS_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_STUFF_POS_SEL_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_STUFF_MODE_set( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_STUFF_MODE_set( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_STUFF_MODE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_STUFF_MODE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_STUFF_MODE_set", N, value );

    /* ((0x00000204 + (N) * 0x20) bits 1) field STUFF_MODE of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    ohfs_insert_reg_FIXED_STUFF_CONFIG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_MODE_MSK,
                                                        OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_MODE_OFF,
                                                        value);
}

static INLINE UINT32 ohfs_insert_field_STUFF_MODE_get( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_STUFF_MODE_get( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_STUFF_MODE_get", N, 95);
    /* ((0x00000204 + (N) * 0x20) bits 1) field STUFF_MODE of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_FIXED_STUFF_CONFIG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_MODE_MSK) >> OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_STUFF_MODE_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_STUFF_EN_set( ohfs_insert_buffer_t *b_ptr,
                                                   ohfs_insert_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_STUFF_EN_set( ohfs_insert_buffer_t *b_ptr,
                                                   ohfs_insert_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_STUFF_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_STUFF_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_STUFF_EN_set", N, value );

    /* ((0x00000204 + (N) * 0x20) bits 0) field STUFF_EN of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    ohfs_insert_reg_FIXED_STUFF_CONFIG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_EN_MSK,
                                                        OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_EN_OFF,
                                                        value);
}

static INLINE UINT32 ohfs_insert_field_STUFF_EN_get( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_STUFF_EN_get( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_STUFF_EN_get", N, 95);
    /* ((0x00000204 + (N) * 0x20) bits 0) field STUFF_EN of register PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG index N=0..95 */
    reg_value = ohfs_insert_reg_FIXED_STUFF_CONFIG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_EN_MSK) >> OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_STUFF_EN_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_RCOH_set( ohfs_insert_buffer_t *b_ptr,
                                               ohfs_insert_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_RCOH_set( ohfs_insert_buffer_t *b_ptr,
                                               ohfs_insert_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_RCOH_set", N, 95);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_RCOH_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_RCOH_set", N, value );

    /* ((0x00000208 + (N) * 0x20) bits 23:0) field RCOH of register PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG index N=0..95 */
    ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_MSK,
                                                         OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_OFF,
                                                         value);
}

static INLINE UINT32 ohfs_insert_field_RCOH_get( ohfs_insert_buffer_t *b_ptr,
                                                 ohfs_insert_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_RCOH_get( ohfs_insert_buffer_t *b_ptr,
                                                 ohfs_insert_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_RCOH_get", N, 95);
    /* ((0x00000208 + (N) * 0x20) bits 23:0) field RCOH of register PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG index N=0..95 */
    reg_value = ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_read( b_ptr,
                                                                h_ptr,
                                                                N);
    value = (reg_value & OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_MSK) >> OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_RCOH_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_RCOH_set( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_RCOH_set( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_RCOH_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_RCOH_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_RCOH_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_RCOH_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000208 + (N) * 0x20) bits 23:0) field RCOH of register PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG index N=0..95 */
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
        /* ((0x00000208 + (N) * 0x20) bits 23:0) field RCOH of register PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG index N=0..95 */
        ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             N,
                                                             subfield_mask << (OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_OFF + subfield_offset),
                                                             OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_RCOH_get( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_RCOH_get( ohfs_insert_buffer_t *b_ptr,
                                                       ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_RCOH_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_RCOH_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_RCOH_get", stop_bit, 23 );
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
    /* ((0x00000208 + (N) * 0x20) bits 23:0) field RCOH of register PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG index N=0..95 */
    reg_value = ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_read( b_ptr,
                                                                h_ptr,
                                                                N);
    field_value = (reg_value & OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_MSK)
                  >> OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_MSK, OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_RCOH_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_XOFF_WM_LEVEL_set( ohfs_insert_buffer_t *b_ptr,
                                                        ohfs_insert_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_XOFF_WM_LEVEL_set( ohfs_insert_buffer_t *b_ptr,
                                                        ohfs_insert_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_XOFF_WM_LEVEL_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_XOFF_WM_LEVEL_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_XOFF_WM_LEVEL_set", N, value );

    /* ((0x0000020c + (N) * 0x20) bits 7:0) field XOFF_WM_LEVEL of register PMC_OHFS_INSERT_REG_XOFF_WM_CFG index N=0..95 */
    ohfs_insert_reg_XOFF_WM_CFG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_MSK,
                                                 OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_OFF,
                                                 value);
}

static INLINE UINT32 ohfs_insert_field_XOFF_WM_LEVEL_get( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_XOFF_WM_LEVEL_get( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_XOFF_WM_LEVEL_get", N, 95);
    /* ((0x0000020c + (N) * 0x20) bits 7:0) field XOFF_WM_LEVEL of register PMC_OHFS_INSERT_REG_XOFF_WM_CFG index N=0..95 */
    reg_value = ohfs_insert_reg_XOFF_WM_CFG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_MSK) >> OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_XOFF_WM_LEVEL_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_XOFF_WM_LEVEL_set( ohfs_insert_buffer_t *b_ptr,
                                                              ohfs_insert_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_XOFF_WM_LEVEL_set( ohfs_insert_buffer_t *b_ptr,
                                                              ohfs_insert_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_XOFF_WM_LEVEL_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_XOFF_WM_LEVEL_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_XOFF_WM_LEVEL_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_XOFF_WM_LEVEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000020c + (N) * 0x20) bits 7:0) field XOFF_WM_LEVEL of register PMC_OHFS_INSERT_REG_XOFF_WM_CFG index N=0..95 */
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
        /* ((0x0000020c + (N) * 0x20) bits 7:0) field XOFF_WM_LEVEL of register PMC_OHFS_INSERT_REG_XOFF_WM_CFG index N=0..95 */
        ohfs_insert_reg_XOFF_WM_CFG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_OFF + subfield_offset),
                                                     OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_XOFF_WM_LEVEL_get( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_XOFF_WM_LEVEL_get( ohfs_insert_buffer_t *b_ptr,
                                                                ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_XOFF_WM_LEVEL_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_XOFF_WM_LEVEL_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_XOFF_WM_LEVEL_get", stop_bit, 7 );
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
    /* ((0x0000020c + (N) * 0x20) bits 7:0) field XOFF_WM_LEVEL of register PMC_OHFS_INSERT_REG_XOFF_WM_CFG index N=0..95 */
    reg_value = ohfs_insert_reg_XOFF_WM_CFG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_MSK)
                  >> OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_MSK, OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_XOFF_WM_LEVEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_MSI_set( ohfs_insert_buffer_t *b_ptr,
                                              ohfs_insert_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_MSI_set( ohfs_insert_buffer_t *b_ptr,
                                              ohfs_insert_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_MSI_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_MSI_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_MSI_set", N, value );

    /* ((0x0000021c + (N) * 0x20) bits 23:16) field MSI of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    ohfs_insert_reg_LO_CFG_REG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_MSK,
                                                OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_OFF,
                                                value);
}

static INLINE UINT32 ohfs_insert_field_MSI_get( ohfs_insert_buffer_t *b_ptr,
                                                ohfs_insert_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_MSI_get( ohfs_insert_buffer_t *b_ptr,
                                                ohfs_insert_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_MSI_get", N, 95);
    /* ((0x0000021c + (N) * 0x20) bits 23:16) field MSI of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    reg_value = ohfs_insert_reg_LO_CFG_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_MSK) >> OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_MSI_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_MSI_set( ohfs_insert_buffer_t *b_ptr,
                                                    ohfs_insert_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_MSI_set( ohfs_insert_buffer_t *b_ptr,
                                                    ohfs_insert_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_MSI_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_MSI_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_MSI_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_MSI_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000021c + (N) * 0x20) bits 23:16) field MSI of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
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
        /* ((0x0000021c + (N) * 0x20) bits 23:16) field MSI of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
        ohfs_insert_reg_LO_CFG_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_OFF + subfield_offset),
                                                    OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_MSI_get( ohfs_insert_buffer_t *b_ptr,
                                                      ohfs_insert_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_MSI_get( ohfs_insert_buffer_t *b_ptr,
                                                      ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_MSI_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_MSI_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_MSI_get", stop_bit, 7 );
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
    /* ((0x0000021c + (N) * 0x20) bits 23:16) field MSI of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    reg_value = ohfs_insert_reg_LO_CFG_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_MSK)
                  >> OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_MSK, OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_MSI_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_TS_LAST_set( ohfs_insert_buffer_t *b_ptr,
                                                  ohfs_insert_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_TS_LAST_set( ohfs_insert_buffer_t *b_ptr,
                                                  ohfs_insert_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_TS_LAST_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_TS_LAST_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_TS_LAST_set", N, value );

    /* ((0x0000021c + (N) * 0x20) bits 12) field TS_LAST of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    ohfs_insert_reg_LO_CFG_REG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_LAST_MSK,
                                                OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_LAST_OFF,
                                                value);
}

static INLINE UINT32 ohfs_insert_field_TS_LAST_get( ohfs_insert_buffer_t *b_ptr,
                                                    ohfs_insert_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_TS_LAST_get( ohfs_insert_buffer_t *b_ptr,
                                                    ohfs_insert_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_TS_LAST_get", N, 95);
    /* ((0x0000021c + (N) * 0x20) bits 12) field TS_LAST of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    reg_value = ohfs_insert_reg_LO_CFG_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_LAST_MSK) >> OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_LAST_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_TS_LAST_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_TS_VALID_set( ohfs_insert_buffer_t *b_ptr,
                                                   ohfs_insert_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_TS_VALID_set( ohfs_insert_buffer_t *b_ptr,
                                                   ohfs_insert_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_TS_VALID_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_TS_VALID_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_TS_VALID_set", N, value );

    /* ((0x0000021c + (N) * 0x20) bits 11) field TS_VALID of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    ohfs_insert_reg_LO_CFG_REG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_VALID_MSK,
                                                OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_VALID_OFF,
                                                value);
}

static INLINE UINT32 ohfs_insert_field_TS_VALID_get( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_TS_VALID_get( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_TS_VALID_get", N, 95);
    /* ((0x0000021c + (N) * 0x20) bits 11) field TS_VALID of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    reg_value = ohfs_insert_reg_LO_CFG_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_VALID_MSK) >> OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_VALID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_TS_VALID_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_MAP_TYPE_set( ohfs_insert_buffer_t *b_ptr,
                                                   ohfs_insert_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_MAP_TYPE_set( ohfs_insert_buffer_t *b_ptr,
                                                   ohfs_insert_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_MAP_TYPE_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_MAP_TYPE_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_MAP_TYPE_set", N, value );

    /* ((0x0000021c + (N) * 0x20) bits 10:8) field MAP_TYPE of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    ohfs_insert_reg_LO_CFG_REG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_MSK,
                                                OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_OFF,
                                                value);
}

static INLINE UINT32 ohfs_insert_field_MAP_TYPE_get( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_MAP_TYPE_get( ohfs_insert_buffer_t *b_ptr,
                                                     ohfs_insert_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_MAP_TYPE_get", N, 95);
    /* ((0x0000021c + (N) * 0x20) bits 10:8) field MAP_TYPE of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    reg_value = ohfs_insert_reg_LO_CFG_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_MSK) >> OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_MAP_TYPE_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_MAP_TYPE_set( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_MAP_TYPE_set( ohfs_insert_buffer_t *b_ptr,
                                                         ohfs_insert_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_MAP_TYPE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_MAP_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_MAP_TYPE_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_MAP_TYPE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000021c + (N) * 0x20) bits 10:8) field MAP_TYPE of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
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
        /* ((0x0000021c + (N) * 0x20) bits 10:8) field MAP_TYPE of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
        ohfs_insert_reg_LO_CFG_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_OFF + subfield_offset),
                                                    OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_MAP_TYPE_get( ohfs_insert_buffer_t *b_ptr,
                                                           ohfs_insert_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_MAP_TYPE_get( ohfs_insert_buffer_t *b_ptr,
                                                           ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_MAP_TYPE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_MAP_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_MAP_TYPE_get", stop_bit, 2 );
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
    /* ((0x0000021c + (N) * 0x20) bits 10:8) field MAP_TYPE of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    reg_value = ohfs_insert_reg_LO_CFG_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_MSK)
                  >> OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_MSK, OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_MAP_TYPE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void ohfs_insert_field_LO_CHID_set( ohfs_insert_buffer_t *b_ptr,
                                                  ohfs_insert_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_LO_CHID_set( ohfs_insert_buffer_t *b_ptr,
                                                  ohfs_insert_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_LO_CHID_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "ohfs_insert_field_LO_CHID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "ohfs_insert_field_LO_CHID_set", N, value );

    /* ((0x0000021c + (N) * 0x20) bits 6:0) field LO_CHID of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    ohfs_insert_reg_LO_CFG_REG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_MSK,
                                                OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_OFF,
                                                value);
}

static INLINE UINT32 ohfs_insert_field_LO_CHID_get( ohfs_insert_buffer_t *b_ptr,
                                                    ohfs_insert_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_LO_CHID_get( ohfs_insert_buffer_t *b_ptr,
                                                    ohfs_insert_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_LO_CHID_get", N, 95);
    /* ((0x0000021c + (N) * 0x20) bits 6:0) field LO_CHID of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    reg_value = ohfs_insert_reg_LO_CFG_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_MSK) >> OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "ohfs_insert_field_LO_CHID_get", N, value );

    return value;
}
static INLINE void ohfs_insert_field_range_LO_CHID_set( ohfs_insert_buffer_t *b_ptr,
                                                        ohfs_insert_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void ohfs_insert_field_range_LO_CHID_set( ohfs_insert_buffer_t *b_ptr,
                                                        ohfs_insert_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_LO_CHID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_LO_CHID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_LO_CHID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_LO_CHID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000021c + (N) * 0x20) bits 6:0) field LO_CHID of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
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
        /* ((0x0000021c + (N) * 0x20) bits 6:0) field LO_CHID of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
        ohfs_insert_reg_LO_CFG_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_OFF + subfield_offset),
                                                    OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 ohfs_insert_field_range_LO_CHID_get( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 ohfs_insert_field_range_LO_CHID_get( ohfs_insert_buffer_t *b_ptr,
                                                          ohfs_insert_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "ohfs_insert_field_range_LO_CHID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "ohfs_insert_field_range_LO_CHID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "ohfs_insert_field_range_LO_CHID_get", stop_bit, 6 );
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
    /* ((0x0000021c + (N) * 0x20) bits 6:0) field LO_CHID of register PMC_OHFS_INSERT_REG_LO_CFG_REG index N=0..95 */
    reg_value = ohfs_insert_reg_LO_CFG_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_MSK)
                  >> OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_MSK, OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "ohfs_insert_field_range_LO_CHID_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OHFS_INSERT_IO_INLINE_H */
