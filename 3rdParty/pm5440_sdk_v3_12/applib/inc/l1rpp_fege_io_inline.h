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
 *     and register accessor functions for the l1rpp_fege block
 *****************************************************************************/
#ifndef _L1RPP_FEGE_IO_INLINE_H
#define _L1RPP_FEGE_IO_INLINE_H

#include "enet_fege_loc.h"
#include "l1rpp_fege_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define L1RPP_FEGE_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for l1rpp_fege
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
} l1rpp_fege_buffer_t;
static INLINE void l1rpp_fege_buffer_init( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_buffer_init( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "l1rpp_fege_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void l1rpp_fege_buffer_flush( l1rpp_fege_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_buffer_flush( l1rpp_fege_buffer_t *b_ptr )
{
    IOLOG( "l1rpp_fege_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 l1rpp_fege_reg_read( l1rpp_fege_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_read( l1rpp_fege_buffer_t *b_ptr,
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
static INLINE void l1rpp_fege_reg_write( l1rpp_fege_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_write( l1rpp_fege_buffer_t *b_ptr,
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

static INLINE void l1rpp_fege_field_set( l1rpp_fege_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 mask,
                                         UINT32 unused_mask,
                                         UINT32 ofs,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_set( l1rpp_fege_buffer_t *b_ptr,
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

static INLINE void l1rpp_fege_action_on_write_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_action_on_write_field_set( l1rpp_fege_buffer_t *b_ptr,
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

static INLINE void l1rpp_fege_burst_read( l1rpp_fege_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_burst_read( l1rpp_fege_buffer_t *b_ptr,
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

static INLINE void l1rpp_fege_burst_write( l1rpp_fege_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_burst_write( l1rpp_fege_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_poll( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                    UINT32 max_count,
                                                    UINT32 *num_failed_polls,
                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_poll( l1rpp_fege_buffer_t *b_ptr,
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
 *  register access functions for l1rpp_fege
 * ==================================================================================
 */

static INLINE void l1rpp_fege_reg_CFG_AND_STAT_write( l1rpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_CFG_AND_STAT_write( l1rpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_CFG_AND_STAT_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_CFG_AND_STAT,
                          value);
}

static INLINE void l1rpp_fege_reg_CFG_AND_STAT_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_CFG_AND_STAT_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_CFG_AND_STAT_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_CFG_AND_STAT,
                          mask,
                          PMC_L1RPP_FEGE_REG_CFG_AND_STAT_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_CFG_AND_STAT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_CFG_AND_STAT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_CFG_AND_STAT);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_CFG_AND_STAT_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_write( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_write( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG,
                          value);
}

static INLINE void l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG,
                          mask,
                          PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_write( l1rpp_fege_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_write( l1rpp_fege_buffer_t *b_ptr,
                                                                        enet_fege_handle_t *h_ptr,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG,
                          value);
}

static INLINE void l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG,
                          mask,
                          PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_MII_CTRL_write( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_MII_CTRL_write( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_MII_CTRL_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_MII_CTRL,
                          value);
}

static INLINE void l1rpp_fege_reg_MII_CTRL_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_MII_CTRL_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_MII_CTRL_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_MII_CTRL,
                          mask,
                          PMC_L1RPP_FEGE_REG_MII_CTRL_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_MII_CTRL_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_MII_CTRL_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_MII_CTRL);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_MII_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_write( l1rpp_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_write( l1rpp_fege_buffer_t *b_ptr,
                                                                            enet_fege_handle_t *h_ptr,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT,
                          value);
}

static INLINE void l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                                enet_fege_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                                enet_fege_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT,
                          mask,
                          PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_write( l1rpp_fege_buffer_t *b_ptr,
                                                                           enet_fege_handle_t *h_ptr,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_write( l1rpp_fege_buffer_t *b_ptr,
                                                                           enet_fege_handle_t *h_ptr,
                                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX,
                          value);
}

static INLINE void l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                               enet_fege_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                               enet_fege_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX,
                          mask,
                          PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_write( l1rpp_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_write( l1rpp_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE,
                          value);
}

static INLINE void l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                       enet_fege_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE,
                          mask,
                          PMC_L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_LINK_TIME_DURATION_write( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_LINK_TIME_DURATION_write( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_LINK_TIME_DURATION_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_LINK_TIME_DURATION,
                          value);
}

static INLINE void l1rpp_fege_reg_LINK_TIME_DURATION_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_LINK_TIME_DURATION_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_LINK_TIME_DURATION_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_LINK_TIME_DURATION,
                          mask,
                          PMC_L1RPP_FEGE_REG_LINK_TIME_DURATION_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_LINK_TIME_DURATION_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_LINK_TIME_DURATION_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_LINK_TIME_DURATION);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_LINK_TIME_DURATION_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_SGMII_GMII_MODE_write( l1rpp_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_SGMII_GMII_MODE_write( l1rpp_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_SGMII_GMII_MODE_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_SGMII_GMII_MODE,
                          value);
}

static INLINE void l1rpp_fege_reg_SGMII_GMII_MODE_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_SGMII_GMII_MODE_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_SGMII_GMII_MODE_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_SGMII_GMII_MODE,
                          mask,
                          PMC_L1RPP_FEGE_REG_SGMII_GMII_MODE_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_SGMII_GMII_MODE_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_SGMII_GMII_MODE_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_SGMII_GMII_MODE);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_SGMII_GMII_MODE_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_COMMA_DET_write( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_COMMA_DET_write( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_COMMA_DET_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_COMMA_DET,
                          value);
}

static INLINE void l1rpp_fege_reg_COMMA_DET_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_COMMA_DET_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_COMMA_DET_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_COMMA_DET,
                          mask,
                          PMC_L1RPP_FEGE_REG_COMMA_DET_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_COMMA_DET_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_COMMA_DET_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_COMMA_DET);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_COMMA_DET_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_PMON_UPDATE_write( l1rpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_PMON_UPDATE_write( l1rpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_PMON_UPDATE_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_PMON_UPDATE,
                          value);
}

static INLINE void l1rpp_fege_reg_PMON_UPDATE_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_PMON_UPDATE_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_PMON_UPDATE_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_PMON_UPDATE,
                          mask,
                          PMC_L1RPP_FEGE_REG_PMON_UPDATE_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_PMON_UPDATE_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_PMON_UPDATE_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_PMON_UPDATE);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_PMON_UPDATE_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_MIN_LINK_TIME_DURATION_write( l1rpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_MIN_LINK_TIME_DURATION_write( l1rpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_MIN_LINK_TIME_DURATION_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION,
                          value);
}

static INLINE void l1rpp_fege_reg_MIN_LINK_TIME_DURATION_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_MIN_LINK_TIME_DURATION_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_MIN_LINK_TIME_DURATION_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION,
                          mask,
                          PMC_L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_MIN_LINK_TIME_DURATION_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_MIN_LINK_TIME_DURATION_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_MIN_LINK_TIME_DURATION_read", reg_value);
    return reg_value;
}

static INLINE void l1rpp_fege_reg_INT_write( l1rpp_fege_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_INT_write( l1rpp_fege_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_INT_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_L1RPP_FEGE_REG_INT,
                          value);
}

static INLINE void l1rpp_fege_reg_INT_action_on_write_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_INT_action_on_write_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_INT_action_on_write_field_set", mask, ofs, value );
    l1rpp_fege_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_L1RPP_FEGE_REG_INT,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 l1rpp_fege_reg_INT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_INT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_L1RPP_FEGE_REG_INT);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_reg_INT_poll( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_reg_INT_poll( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "l1rpp_fege_reg_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return l1rpp_fege_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_L1RPP_FEGE_REG_INT,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void l1rpp_fege_reg_INT_EN_write( l1rpp_fege_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_INT_EN_write( l1rpp_fege_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "l1rpp_fege_reg_INT_EN_write", value );
    l1rpp_fege_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_L1RPP_FEGE_REG_INT_EN,
                          value);
}

static INLINE void l1rpp_fege_reg_INT_EN_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_reg_INT_EN_field_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "l1rpp_fege_reg_INT_EN_field_set", mask, ofs, value );
    l1rpp_fege_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_L1RPP_FEGE_REG_INT_EN,
                          mask,
                          PMC_L1RPP_FEGE_REG_INT_EN_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 l1rpp_fege_reg_INT_EN_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_INT_EN_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_L1RPP_FEGE_REG_INT_EN);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE UINT32 l1rpp_fege_reg_INT_STAT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_INT_STAT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_L1RPP_FEGE_REG_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_reg_INT_STAT_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_reg_INT_STAT_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "l1rpp_fege_reg_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return l1rpp_fege_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_L1RPP_FEGE_REG_INT_STAT,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 l1rpp_fege_reg_PMON_CNT_0_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_PMON_CNT_0_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_L1RPP_FEGE_REG_PMON_CNT_0);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_PMON_CNT_0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 l1rpp_fege_reg_PMON_CNT_1_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_PMON_CNT_1_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_L1RPP_FEGE_REG_PMON_CNT_1);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_PMON_CNT_1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 l1rpp_fege_reg_PMON_CNT_2_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_PMON_CNT_2_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_L1RPP_FEGE_REG_PMON_CNT_2);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_PMON_CNT_2_read", reg_value);
    return reg_value;
}

static INLINE UINT32 l1rpp_fege_reg_MII_STAT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_MII_STAT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_L1RPP_FEGE_REG_MII_STAT);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_MII_STAT_read", reg_value);
    return reg_value;
}

static INLINE UINT32 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_read", reg_value);
    return reg_value;
}

static INLINE UINT32 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_EXPANSION_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_EXPANSION_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_MII_AUTO_NEGOTIATION_EXPANSION_read", reg_value);
    return reg_value;
}

static INLINE UINT32 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_read", reg_value);
    return reg_value;
}

static INLINE UINT32 l1rpp_fege_reg_BIT_ALIGNMENT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_reg_BIT_ALIGNMENT_read( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = l1rpp_fege_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_L1RPP_FEGE_REG_BIT_ALIGNMENT);

    IOLOG( "%s -> 0x%08x;", "l1rpp_fege_reg_BIT_ALIGNMENT_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void l1rpp_fege_field_AUTO_SENSE_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_AUTO_SENSE_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_AUTO_SENSE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_AUTO_SENSE_set", value );

    /* (0x0000008c bits 11) field AUTO_SENSE of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    l1rpp_fege_reg_CFG_AND_STAT_field_set( b_ptr,
                                           h_ptr,
                                           L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AUTO_SENSE_MSK,
                                           L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AUTO_SENSE_OFF,
                                           value);
}

static INLINE UINT32 l1rpp_fege_field_AUTO_SENSE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_AUTO_SENSE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 11) field AUTO_SENSE of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    reg_value = l1rpp_fege_reg_CFG_AND_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AUTO_SENSE_MSK) >> L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AUTO_SENSE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_AUTO_SENSE_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_MANUAL_ACK_EN_set( l1rpp_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_MANUAL_ACK_EN_set( l1rpp_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_MANUAL_ACK_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_MANUAL_ACK_EN_set", value );

    /* (0x0000008c bits 10) field MANUAL_ACK_EN of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    l1rpp_fege_reg_CFG_AND_STAT_field_set( b_ptr,
                                           h_ptr,
                                           L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MANUAL_ACK_EN_MSK,
                                           L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MANUAL_ACK_EN_OFF,
                                           value);
}

static INLINE UINT32 l1rpp_fege_field_MANUAL_ACK_EN_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_MANUAL_ACK_EN_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 10) field MANUAL_ACK_EN of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    reg_value = l1rpp_fege_reg_CFG_AND_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MANUAL_ACK_EN_MSK) >> L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MANUAL_ACK_EN_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_MANUAL_ACK_EN_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_MODE_set( l1rpp_fege_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_MODE_set( l1rpp_fege_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_MODE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_MODE_set", value );

    /* (0x0000008c bits 6:4) bits 0:2 use field MODE of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    l1rpp_fege_reg_CFG_AND_STAT_field_set( b_ptr,
                                           h_ptr,
                                           L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_MSK,
                                           L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_OFF,
                                           value);
}

static INLINE UINT32 l1rpp_fege_field_MODE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_MODE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 6:4) bits 0:2 use field MODE of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    reg_value = l1rpp_fege_reg_CFG_AND_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_MSK) >> L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_MODE_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_range_MODE_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_range_MODE_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_MODE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_MODE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_MODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000008c bits 6:4) bits 0:2 use field MODE of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
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
        /* (0x0000008c bits 6:4) bits 0:2 use field MODE of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
        l1rpp_fege_reg_CFG_AND_STAT_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_OFF + subfield_offset),
                                               L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 l1rpp_fege_field_range_MODE_get( l1rpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_MODE_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_MODE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_MODE_get", stop_bit, 2 );
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
    /* (0x0000008c bits 6:4) bits 0:2 use field MODE of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    reg_value = l1rpp_fege_reg_CFG_AND_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_MSK)
                  >> L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_MSK, L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_MODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1rpp_fege_field_L1RPP_EN_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_L1RPP_EN_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_L1RPP_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_L1RPP_EN_set", value );

    /* (0x0000008c bits 0) field L1RPP_EN of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    l1rpp_fege_reg_CFG_AND_STAT_field_set( b_ptr,
                                           h_ptr,
                                           L1RPP_FEGE_REG_CFG_AND_STAT_BIT_L1RPP_EN_MSK,
                                           L1RPP_FEGE_REG_CFG_AND_STAT_BIT_L1RPP_EN_OFF,
                                           value);
}

static INLINE UINT32 l1rpp_fege_field_L1RPP_EN_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_L1RPP_EN_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 0) field L1RPP_EN of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    reg_value = l1rpp_fege_reg_CFG_AND_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_CFG_AND_STAT_BIT_L1RPP_EN_MSK) >> L1RPP_FEGE_REG_CFG_AND_STAT_BIT_L1RPP_EN_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_L1RPP_EN_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_LS_RESET_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LS_RESET_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LS_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LS_RESET_set", value );

    /* (0x00000090 bits 14) field LS_RESET of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
    l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LS_RESET_MSK,
                                                  L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LS_RESET_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_LS_RESET_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LS_RESET_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 14) field LS_RESET of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
    reg_value = l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LS_RESET_MSK) >> L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LS_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LS_RESET_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_RESYNC_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_RESYNC_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_RESYNC_COUNT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_RESYNC_COUNT_set", value );

    /* (0x00000090 bits 11:8) bits 0:3 use field RESYNC_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
    l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_MSK,
                                                  L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_RESYNC_COUNT_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_RESYNC_COUNT_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 11:8) bits 0:3 use field RESYNC_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
    reg_value = l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_MSK) >> L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_RESYNC_COUNT_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_range_RESYNC_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_range_RESYNC_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_RESYNC_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_RESYNC_COUNT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_RESYNC_COUNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000090 bits 11:8) bits 0:3 use field RESYNC_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
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
        /* (0x00000090 bits 11:8) bits 0:3 use field RESYNC_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
        l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_OFF + subfield_offset),
                                                      L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 l1rpp_fege_field_range_RESYNC_COUNT_get( l1rpp_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_RESYNC_COUNT_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_RESYNC_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_RESYNC_COUNT_get", stop_bit, 3 );
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
    /* (0x00000090 bits 11:8) bits 0:3 use field RESYNC_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
    reg_value = l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_MSK)
                  >> L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_MSK, L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_RESYNC_COUNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1rpp_fege_field_LOS_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LOS_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LOS_COUNT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LOS_COUNT_set", value );

    /* (0x00000090 bits 7:4) bits 0:3 use field LOS_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
    l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_MSK,
                                                  L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_LOS_COUNT_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LOS_COUNT_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 7:4) bits 0:3 use field LOS_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
    reg_value = l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_MSK) >> L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LOS_COUNT_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_range_LOS_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_range_LOS_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_LOS_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_LOS_COUNT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_LOS_COUNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000090 bits 7:4) bits 0:3 use field LOS_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
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
        /* (0x00000090 bits 7:4) bits 0:3 use field LOS_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
        l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_OFF + subfield_offset),
                                                      L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 l1rpp_fege_field_range_LOS_COUNT_get( l1rpp_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_LOS_COUNT_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_LOS_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_LOS_COUNT_get", stop_bit, 3 );
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
    /* (0x00000090 bits 7:4) bits 0:3 use field LOS_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
    reg_value = l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_MSK)
                  >> L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_MSK, L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_LOS_COUNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1rpp_fege_field_SYNC_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_SYNC_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_SYNC_COUNT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_SYNC_COUNT_set", value );

    /* (0x00000090 bits 3:0) bits 0:3 use field SYNC_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
    l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_MSK,
                                                  L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_SYNC_COUNT_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_SYNC_COUNT_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 3:0) bits 0:3 use field SYNC_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
    reg_value = l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_MSK) >> L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_SYNC_COUNT_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_range_SYNC_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_range_SYNC_COUNT_set( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_SYNC_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_SYNC_COUNT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_SYNC_COUNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000090 bits 3:0) bits 0:3 use field SYNC_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
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
        /* (0x00000090 bits 3:0) bits 0:3 use field SYNC_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
        l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_OFF + subfield_offset),
                                                      L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 l1rpp_fege_field_range_SYNC_COUNT_get( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_SYNC_COUNT_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_SYNC_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_SYNC_COUNT_get", stop_bit, 3 );
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
    /* (0x00000090 bits 3:0) bits 0:3 use field SYNC_COUNT of register PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG */
    reg_value = l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_MSK)
                  >> L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_MSK, L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_SYNC_COUNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1rpp_fege_field_PTRN_SEL_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_PTRN_SEL_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_PTRN_SEL_set", value, 3);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_PTRN_SEL_set", value );

    /* (0x00000094 bits 11:10) bits 0:1 use field PTRN_SEL of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
    l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_field_set( b_ptr,
                                                             h_ptr,
                                                             L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_MSK,
                                                             L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_OFF,
                                                             value);
}

static INLINE UINT32 l1rpp_fege_field_PTRN_SEL_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_PTRN_SEL_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 11:10) bits 0:1 use field PTRN_SEL of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
    reg_value = l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read( b_ptr,
                                                                    h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_MSK) >> L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_PTRN_SEL_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_range_PTRN_SEL_set( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_range_PTRN_SEL_set( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_PTRN_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_PTRN_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_PTRN_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000094 bits 11:10) bits 0:1 use field PTRN_SEL of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
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
        /* (0x00000094 bits 11:10) bits 0:1 use field PTRN_SEL of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
        l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_OFF + subfield_offset),
                                                                 L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 l1rpp_fege_field_range_PTRN_SEL_get( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_PTRN_SEL_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_PTRN_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_PTRN_SEL_get", stop_bit, 1 );
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
    /* (0x00000094 bits 11:10) bits 0:1 use field PTRN_SEL of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
    reg_value = l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read( b_ptr,
                                                                    h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_MSK)
                  >> L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_MSK, L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_PTRN_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1rpp_fege_field_CUS_PTRN_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_CUS_PTRN_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_CUS_PTRN_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_CUS_PTRN_set", value );

    /* (0x00000094 bits 9:0) bits 0:9 use field CUS_PTRN of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
    l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_field_set( b_ptr,
                                                             h_ptr,
                                                             L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_MSK,
                                                             L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_OFF,
                                                             value);
}

static INLINE UINT32 l1rpp_fege_field_CUS_PTRN_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_CUS_PTRN_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 9:0) bits 0:9 use field CUS_PTRN of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
    reg_value = l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read( b_ptr,
                                                                    h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_MSK) >> L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_CUS_PTRN_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_range_CUS_PTRN_set( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_range_CUS_PTRN_set( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_CUS_PTRN_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_CUS_PTRN_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_CUS_PTRN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000094 bits 9:0) bits 0:9 use field CUS_PTRN of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
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
        /* (0x00000094 bits 9:0) bits 0:9 use field CUS_PTRN of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
        l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_OFF + subfield_offset),
                                                                 L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 l1rpp_fege_field_range_CUS_PTRN_get( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_CUS_PTRN_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_CUS_PTRN_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_CUS_PTRN_get", stop_bit, 9 );
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
    /* (0x00000094 bits 9:0) bits 0:9 use field CUS_PTRN of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
    reg_value = l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read( b_ptr,
                                                                    h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_MSK)
                  >> L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_MSK, L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_CUS_PTRN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1rpp_fege_field_AN_ENABLE_set( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_AN_ENABLE_set( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_AN_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_AN_ENABLE_set", value );

    /* (0x00000098 bits 12) field AN_ENABLE of register PMC_L1RPP_FEGE_REG_MII_CTRL */
    l1rpp_fege_reg_MII_CTRL_field_set( b_ptr,
                                       h_ptr,
                                       L1RPP_FEGE_REG_MII_CTRL_BIT_AN_ENABLE_MSK,
                                       L1RPP_FEGE_REG_MII_CTRL_BIT_AN_ENABLE_OFF,
                                       value);
}

static INLINE UINT32 l1rpp_fege_field_AN_ENABLE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_AN_ENABLE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000098 bits 12) field AN_ENABLE of register PMC_L1RPP_FEGE_REG_MII_CTRL */
    reg_value = l1rpp_fege_reg_MII_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_CTRL_BIT_AN_ENABLE_MSK) >> L1RPP_FEGE_REG_MII_CTRL_BIT_AN_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_AN_ENABLE_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_AN_ABLTY_ADV_set( l1rpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_AN_ABLTY_ADV_set( l1rpp_fege_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_AN_ABLTY_ADV_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_AN_ABLTY_ADV_set", value );

    /* (0x000000a0 bits 15:0) bits 0:15 use field AN_ABLTY_ADV of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT */
    l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_field_set( b_ptr,
                                                                 h_ptr,
                                                                 L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_MSK,
                                                                 L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_OFF,
                                                                 value);
}

static INLINE UINT32 l1rpp_fege_field_AN_ABLTY_ADV_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_AN_ABLTY_ADV_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a0 bits 15:0) bits 0:15 use field AN_ABLTY_ADV of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_read( b_ptr,
                                                                        h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_AN_ABLTY_ADV_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_range_AN_ABLTY_ADV_set( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_range_AN_ABLTY_ADV_set( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_AN_ABLTY_ADV_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_AN_ABLTY_ADV_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_AN_ABLTY_ADV_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a0 bits 15:0) bits 0:15 use field AN_ABLTY_ADV of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT */
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
        /* (0x000000a0 bits 15:0) bits 0:15 use field AN_ABLTY_ADV of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT */
        l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_field_set( b_ptr,
                                                                     h_ptr,
                                                                     subfield_mask << (L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_OFF + subfield_offset),
                                                                     L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 l1rpp_fege_field_range_AN_ABLTY_ADV_get( l1rpp_fege_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_AN_ABLTY_ADV_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_AN_ABLTY_ADV_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_AN_ABLTY_ADV_get", stop_bit, 15 );
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
    /* (0x000000a0 bits 15:0) bits 0:15 use field AN_ABLTY_ADV of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_read( b_ptr,
                                                                        h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_MSK)
                  >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_MSK, L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_AN_ABLTY_ADV_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1rpp_fege_field_LOC_NP_NEXT_PAGE_set( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LOC_NP_NEXT_PAGE_set( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LOC_NP_NEXT_PAGE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LOC_NP_NEXT_PAGE_set", value );

    /* (0x000000ac bits 15) field LOC_NP_NEXT_PAGE of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
    l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_field_set( b_ptr,
                                                                h_ptr,
                                                                L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_NEXT_PAGE_MSK,
                                                                L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_NEXT_PAGE_OFF,
                                                                value);
}

static INLINE UINT32 l1rpp_fege_field_LOC_NP_NEXT_PAGE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LOC_NP_NEXT_PAGE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000ac bits 15) field LOC_NP_NEXT_PAGE of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_NEXT_PAGE_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_NEXT_PAGE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LOC_NP_NEXT_PAGE_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_LOC_NP_MSG_PAGE_set( l1rpp_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LOC_NP_MSG_PAGE_set( l1rpp_fege_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LOC_NP_MSG_PAGE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LOC_NP_MSG_PAGE_set", value );

    /* (0x000000ac bits 13) field LOC_NP_MSG_PAGE of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
    l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_field_set( b_ptr,
                                                                h_ptr,
                                                                L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_MSG_PAGE_MSK,
                                                                L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_MSG_PAGE_OFF,
                                                                value);
}

static INLINE UINT32 l1rpp_fege_field_LOC_NP_MSG_PAGE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LOC_NP_MSG_PAGE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000ac bits 13) field LOC_NP_MSG_PAGE of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_MSG_PAGE_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_MSG_PAGE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LOC_NP_MSG_PAGE_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_LOC_NP_ACK2_set( l1rpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LOC_NP_ACK2_set( l1rpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LOC_NP_ACK2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LOC_NP_ACK2_set", value );

    /* (0x000000ac bits 12) field LOC_NP_ACK2 of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
    l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_field_set( b_ptr,
                                                                h_ptr,
                                                                L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_ACK2_MSK,
                                                                L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_ACK2_OFF,
                                                                value);
}

static INLINE UINT32 l1rpp_fege_field_LOC_NP_ACK2_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LOC_NP_ACK2_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000ac bits 12) field LOC_NP_ACK2 of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_ACK2_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_ACK2_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LOC_NP_ACK2_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_LOC_NP_CODE_set( l1rpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LOC_NP_CODE_set( l1rpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 2047)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LOC_NP_CODE_set", value, 2047);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LOC_NP_CODE_set", value );

    /* (0x000000ac bits 10:0) bits 0:10 use field LOC_NP_CODE_FIELD of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
    l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_field_set( b_ptr,
                                                                h_ptr,
                                                                L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_MSK,
                                                                L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_OFF,
                                                                value);
}

static INLINE UINT32 l1rpp_fege_field_LOC_NP_CODE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LOC_NP_CODE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000ac bits 10:0) bits 0:10 use field LOC_NP_CODE_FIELD of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LOC_NP_CODE_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_range_LOC_NP_CODE_set( l1rpp_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_range_LOC_NP_CODE_set( l1rpp_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_LOC_NP_CODE_set", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_LOC_NP_CODE_set", stop_bit, 10 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_LOC_NP_CODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000ac bits 10:0) bits 0:10 use field LOC_NP_CODE_FIELD of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
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
        if (stop_bit < 10) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 10;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000ac bits 10:0) bits 0:10 use field LOC_NP_CODE_FIELD of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
        l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_field_set( b_ptr,
                                                                    h_ptr,
                                                                    subfield_mask << (L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_OFF + subfield_offset),
                                                                    L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_OFF + subfield_offset,
                                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 l1rpp_fege_field_range_LOC_NP_CODE_get( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_LOC_NP_CODE_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_LOC_NP_CODE_get", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_LOC_NP_CODE_get", stop_bit, 10 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 10) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 10;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000000ac bits 10:0) bits 0:10 use field LOC_NP_CODE_FIELD of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_read( b_ptr,
                                                                       h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_MSK)
                  >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_MSK, L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_LOC_NP_CODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1rpp_fege_field_LINK_TIMER_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LINK_TIMER_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LINK_TIMER_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LINK_TIMER_set", value );

    /* (0x000000bc bits 15:0) bits 0:15 use field LINK_TIMER of register PMC_L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE */
    l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_field_set( b_ptr,
                                                        h_ptr,
                                                        L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_MSK,
                                                        L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_OFF,
                                                        value);
}

static INLINE UINT32 l1rpp_fege_field_LINK_TIMER_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LINK_TIMER_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000bc bits 15:0) bits 0:15 use field LINK_TIMER of register PMC_L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE */
    reg_value = l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_MSK) >> L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LINK_TIMER_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_range_LINK_TIMER_set( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_range_LINK_TIMER_set( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_LINK_TIMER_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_LINK_TIMER_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_LINK_TIMER_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000bc bits 15:0) bits 0:15 use field LINK_TIMER of register PMC_L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE */
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
        /* (0x000000bc bits 15:0) bits 0:15 use field LINK_TIMER of register PMC_L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE */
        l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_OFF + subfield_offset),
                                                            L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 l1rpp_fege_field_range_LINK_TIMER_get( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_LINK_TIMER_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_LINK_TIMER_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_LINK_TIMER_get", stop_bit, 15 );
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
    /* (0x000000bc bits 15:0) bits 0:15 use field LINK_TIMER of register PMC_L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE */
    reg_value = l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_MSK)
                  >> L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_MSK, L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_LINK_TIMER_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1rpp_fege_field_LINK_TIME_DUR_set( l1rpp_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LINK_TIME_DUR_set( l1rpp_fege_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 16383)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LINK_TIME_DUR_set", value, 16383);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LINK_TIME_DUR_set", value );

    /* (0x000000c0 bits 13:0) bits 0:13 use field LINK_TIME_DUR of register PMC_L1RPP_FEGE_REG_LINK_TIME_DURATION */
    l1rpp_fege_reg_LINK_TIME_DURATION_field_set( b_ptr,
                                                 h_ptr,
                                                 L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_MSK,
                                                 L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_OFF,
                                                 value);
}

static INLINE UINT32 l1rpp_fege_field_LINK_TIME_DUR_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LINK_TIME_DUR_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c0 bits 13:0) bits 0:13 use field LINK_TIME_DUR of register PMC_L1RPP_FEGE_REG_LINK_TIME_DURATION */
    reg_value = l1rpp_fege_reg_LINK_TIME_DURATION_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_MSK) >> L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LINK_TIME_DUR_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_range_LINK_TIME_DUR_set( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_range_LINK_TIME_DUR_set( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_LINK_TIME_DUR_set", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_LINK_TIME_DUR_set", stop_bit, 13 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_LINK_TIME_DUR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000c0 bits 13:0) bits 0:13 use field LINK_TIME_DUR of register PMC_L1RPP_FEGE_REG_LINK_TIME_DURATION */
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
        /* (0x000000c0 bits 13:0) bits 0:13 use field LINK_TIME_DUR of register PMC_L1RPP_FEGE_REG_LINK_TIME_DURATION */
        l1rpp_fege_reg_LINK_TIME_DURATION_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_OFF + subfield_offset),
                                                     L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 l1rpp_fege_field_range_LINK_TIME_DUR_get( l1rpp_fege_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_LINK_TIME_DUR_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_LINK_TIME_DUR_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_LINK_TIME_DUR_get", stop_bit, 13 );
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
    /* (0x000000c0 bits 13:0) bits 0:13 use field LINK_TIME_DUR of register PMC_L1RPP_FEGE_REG_LINK_TIME_DURATION */
    reg_value = l1rpp_fege_reg_LINK_TIME_DURATION_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_MSK)
                  >> L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_MSK, L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_LINK_TIME_DUR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void l1rpp_fege_field_MODE_SEL_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_MODE_SEL_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_MODE_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_MODE_SEL_set", value );

    /* (0x000000c4 bits 15) field MODE_SEL of register PMC_L1RPP_FEGE_REG_SGMII_GMII_MODE */
    l1rpp_fege_reg_SGMII_GMII_MODE_field_set( b_ptr,
                                              h_ptr,
                                              L1RPP_FEGE_REG_SGMII_GMII_MODE_BIT_MODE_SEL_MSK,
                                              L1RPP_FEGE_REG_SGMII_GMII_MODE_BIT_MODE_SEL_OFF,
                                              value);
}

static INLINE UINT32 l1rpp_fege_field_MODE_SEL_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_MODE_SEL_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 15) field MODE_SEL of register PMC_L1RPP_FEGE_REG_SGMII_GMII_MODE */
    reg_value = l1rpp_fege_reg_SGMII_GMII_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_SGMII_GMII_MODE_BIT_MODE_SEL_MSK) >> L1RPP_FEGE_REG_SGMII_GMII_MODE_BIT_MODE_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_MODE_SEL_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_EN_CDET_SEL_set( l1rpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_EN_CDET_SEL_set( l1rpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_EN_CDET_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_EN_CDET_SEL_set", value );

    /* (0x000000c8 bits 0) field EN_CDET_SEL of register PMC_L1RPP_FEGE_REG_COMMA_DET */
    l1rpp_fege_reg_COMMA_DET_field_set( b_ptr,
                                        h_ptr,
                                        L1RPP_FEGE_REG_COMMA_DET_BIT_EN_CDET_SEL_MSK,
                                        L1RPP_FEGE_REG_COMMA_DET_BIT_EN_CDET_SEL_OFF,
                                        value);
}

static INLINE UINT32 l1rpp_fege_field_EN_CDET_SEL_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_EN_CDET_SEL_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c8 bits 0) field EN_CDET_SEL of register PMC_L1RPP_FEGE_REG_COMMA_DET */
    reg_value = l1rpp_fege_reg_COMMA_DET_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_COMMA_DET_BIT_EN_CDET_SEL_MSK) >> L1RPP_FEGE_REG_COMMA_DET_BIT_EN_CDET_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_EN_CDET_SEL_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_PMON_UPDATE_set( l1rpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_PMON_UPDATE_set( l1rpp_fege_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_PMON_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_PMON_UPDATE_set", value );

    /* (0x000000d8 bits 0) field PMON_UPDATE of register PMC_L1RPP_FEGE_REG_PMON_UPDATE */
    l1rpp_fege_reg_PMON_UPDATE_field_set( b_ptr,
                                          h_ptr,
                                          L1RPP_FEGE_REG_PMON_UPDATE_BIT_PMON_UPDATE_MSK,
                                          L1RPP_FEGE_REG_PMON_UPDATE_BIT_PMON_UPDATE_OFF,
                                          value);
}

static INLINE UINT32 l1rpp_fege_field_PMON_UPDATE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_PMON_UPDATE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d8 bits 0) field PMON_UPDATE of register PMC_L1RPP_FEGE_REG_PMON_UPDATE */
    reg_value = l1rpp_fege_reg_PMON_UPDATE_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_PMON_UPDATE_BIT_PMON_UPDATE_MSK) >> L1RPP_FEGE_REG_PMON_UPDATE_BIT_PMON_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_PMON_UPDATE_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_MIN_LINK_TIME_DUR_set( l1rpp_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_MIN_LINK_TIME_DUR_set( l1rpp_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 16383)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_MIN_LINK_TIME_DUR_set", value, 16383);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_MIN_LINK_TIME_DUR_set", value );

    /* (0x000000e0 bits 13:0) bits 0:13 use field MIN_LINK_TIME_DUR of register PMC_L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION */
    l1rpp_fege_reg_MIN_LINK_TIME_DURATION_field_set( b_ptr,
                                                     h_ptr,
                                                     L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_MSK,
                                                     L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_OFF,
                                                     value);
}

static INLINE UINT32 l1rpp_fege_field_MIN_LINK_TIME_DUR_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_MIN_LINK_TIME_DUR_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e0 bits 13:0) bits 0:13 use field MIN_LINK_TIME_DUR of register PMC_L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION */
    reg_value = l1rpp_fege_reg_MIN_LINK_TIME_DURATION_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_MSK) >> L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_MIN_LINK_TIME_DUR_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_range_MIN_LINK_TIME_DUR_set( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_range_MIN_LINK_TIME_DUR_set( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_MIN_LINK_TIME_DUR_set", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_MIN_LINK_TIME_DUR_set", stop_bit, 13 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_MIN_LINK_TIME_DUR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000e0 bits 13:0) bits 0:13 use field MIN_LINK_TIME_DUR of register PMC_L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION */
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
        /* (0x000000e0 bits 13:0) bits 0:13 use field MIN_LINK_TIME_DUR of register PMC_L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION */
        l1rpp_fege_reg_MIN_LINK_TIME_DURATION_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_OFF + subfield_offset),
                                                         L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 l1rpp_fege_field_range_MIN_LINK_TIME_DUR_get( l1rpp_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_MIN_LINK_TIME_DUR_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_MIN_LINK_TIME_DUR_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_MIN_LINK_TIME_DUR_get", stop_bit, 13 );
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
    /* (0x000000e0 bits 13:0) bits 0:13 use field MIN_LINK_TIME_DUR of register PMC_L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION */
    reg_value = l1rpp_fege_reg_MIN_LINK_TIME_DURATION_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_MSK)
                  >> L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_MSK, L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_MIN_LINK_TIME_DUR_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void l1rpp_fege_field_AN_ABLTY_ADV_UPD_set( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_AN_ABLTY_ADV_UPD_set( l1rpp_fege_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_AN_ABLTY_ADV_UPD_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_AN_ABLTY_ADV_UPD_set", value );

    /* (0x0000008c bits 9) field AN_ABLTY_ADV_UPD of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    l1rpp_fege_reg_CFG_AND_STAT_field_set( b_ptr,
                                           h_ptr,
                                           L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AN_ABLTY_ADV_UPD_MSK,
                                           L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AN_ABLTY_ADV_UPD_OFF,
                                           value);
}

static INLINE UINT32 l1rpp_fege_field_AN_ABLTY_ADV_UPD_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_AN_ABLTY_ADV_UPD_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 9) field AN_ABLTY_ADV_UPD of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    reg_value = l1rpp_fege_reg_CFG_AND_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AN_ABLTY_ADV_UPD_MSK) >> L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AN_ABLTY_ADV_UPD_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_AN_ABLTY_ADV_UPD_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_AN_RESTART_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_AN_RESTART_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_AN_RESTART_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_AN_RESTART_set", value );

    /* (0x00000098 bits 9) field AN_RESTART of register PMC_L1RPP_FEGE_REG_MII_CTRL */
    l1rpp_fege_reg_MII_CTRL_field_set( b_ptr,
                                       h_ptr,
                                       L1RPP_FEGE_REG_MII_CTRL_BIT_AN_RESTART_MSK,
                                       L1RPP_FEGE_REG_MII_CTRL_BIT_AN_RESTART_OFF,
                                       value);
}

static INLINE UINT32 l1rpp_fege_field_AN_RESTART_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_AN_RESTART_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000098 bits 9) field AN_RESTART of register PMC_L1RPP_FEGE_REG_MII_CTRL */
    reg_value = l1rpp_fege_reg_MII_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_CTRL_BIT_AN_RESTART_MSK) >> L1RPP_FEGE_REG_MII_CTRL_BIT_AN_RESTART_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_AN_RESTART_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_both
 * ==================================================================================
 */
static INLINE void l1rpp_fege_field_LINK_MON_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LINK_MON_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LINK_MON_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LINK_MON_I_set_to_clear", value );

    /* (0x00000080 bits 12) field LINK_MON_I of register PMC_L1RPP_FEGE_REG_INT */
    l1rpp_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_INT_BIT_LINK_MON_I_MSK,
                                                  L1RPP_FEGE_REG_INT_BIT_LINK_MON_I_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_LINK_MON_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LINK_MON_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 12) field LINK_MON_I of register PMC_L1RPP_FEGE_REG_INT */
    reg_value = l1rpp_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_BIT_LINK_MON_I_MSK) >> L1RPP_FEGE_REG_INT_BIT_LINK_MON_I_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LINK_MON_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_MON_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_MON_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_LINK_MON_I_poll", value );

    /* (0x00000080 bits 12) field LINK_MON_I of register PMC_L1RPP_FEGE_REG_INT */
    return l1rpp_fege_reg_INT_poll( b_ptr,
                                    h_ptr,
                                    L1RPP_FEGE_REG_INT_BIT_LINK_MON_I_MSK,
                                    (value<<L1RPP_FEGE_REG_INT_BIT_LINK_MON_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void l1rpp_fege_field_FEC_STAT_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_FEC_STAT_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_FEC_STAT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_FEC_STAT_I_set_to_clear", value );

    /* (0x00000080 bits 11) field FEC_STAT_I of register PMC_L1RPP_FEGE_REG_INT */
    l1rpp_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_INT_BIT_FEC_STAT_I_MSK,
                                                  L1RPP_FEGE_REG_INT_BIT_FEC_STAT_I_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_FEC_STAT_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_FEC_STAT_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 11) field FEC_STAT_I of register PMC_L1RPP_FEGE_REG_INT */
    reg_value = l1rpp_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_BIT_FEC_STAT_I_MSK) >> L1RPP_FEGE_REG_INT_BIT_FEC_STAT_I_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_FEC_STAT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_FEC_STAT_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_FEC_STAT_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_FEC_STAT_I_poll", value );

    /* (0x00000080 bits 11) field FEC_STAT_I of register PMC_L1RPP_FEGE_REG_INT */
    return l1rpp_fege_reg_INT_poll( b_ptr,
                                    h_ptr,
                                    L1RPP_FEGE_REG_INT_BIT_FEC_STAT_I_MSK,
                                    (value<<L1RPP_FEGE_REG_INT_BIT_FEC_STAT_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void l1rpp_fege_field_CARRIER_STAT_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_CARRIER_STAT_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_CARRIER_STAT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_CARRIER_STAT_I_set_to_clear", value );

    /* (0x00000080 bits 10) field CARRIER_STAT_I of register PMC_L1RPP_FEGE_REG_INT */
    l1rpp_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_INT_BIT_CARRIER_STAT_I_MSK,
                                                  L1RPP_FEGE_REG_INT_BIT_CARRIER_STAT_I_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_CARRIER_STAT_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_CARRIER_STAT_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 10) field CARRIER_STAT_I of register PMC_L1RPP_FEGE_REG_INT */
    reg_value = l1rpp_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_BIT_CARRIER_STAT_I_MSK) >> L1RPP_FEGE_REG_INT_BIT_CARRIER_STAT_I_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_CARRIER_STAT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_CARRIER_STAT_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_CARRIER_STAT_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_CARRIER_STAT_I_poll", value );

    /* (0x00000080 bits 10) field CARRIER_STAT_I of register PMC_L1RPP_FEGE_REG_INT */
    return l1rpp_fege_reg_INT_poll( b_ptr,
                                    h_ptr,
                                    L1RPP_FEGE_REG_INT_BIT_CARRIER_STAT_I_MSK,
                                    (value<<L1RPP_FEGE_REG_INT_BIT_CARRIER_STAT_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void l1rpp_fege_field_LINK_OK_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LINK_OK_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LINK_OK_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LINK_OK_I_set_to_clear", value );

    /* (0x00000080 bits 4) field LINK_OK_I of register PMC_L1RPP_FEGE_REG_INT */
    l1rpp_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_INT_BIT_LINK_OK_I_MSK,
                                                  L1RPP_FEGE_REG_INT_BIT_LINK_OK_I_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_LINK_OK_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LINK_OK_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 4) field LINK_OK_I of register PMC_L1RPP_FEGE_REG_INT */
    reg_value = l1rpp_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_BIT_LINK_OK_I_MSK) >> L1RPP_FEGE_REG_INT_BIT_LINK_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LINK_OK_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_OK_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_OK_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_LINK_OK_I_poll", value );

    /* (0x00000080 bits 4) field LINK_OK_I of register PMC_L1RPP_FEGE_REG_INT */
    return l1rpp_fege_reg_INT_poll( b_ptr,
                                    h_ptr,
                                    L1RPP_FEGE_REG_INT_BIT_LINK_OK_I_MSK,
                                    (value<<L1RPP_FEGE_REG_INT_BIT_LINK_OK_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void l1rpp_fege_field_SYNC_OK_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_SYNC_OK_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_SYNC_OK_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_SYNC_OK_I_set_to_clear", value );

    /* (0x00000080 bits 0) field SYNC_OK_I of register PMC_L1RPP_FEGE_REG_INT */
    l1rpp_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_INT_BIT_SYNC_OK_I_MSK,
                                                  L1RPP_FEGE_REG_INT_BIT_SYNC_OK_I_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_SYNC_OK_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_SYNC_OK_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 0) field SYNC_OK_I of register PMC_L1RPP_FEGE_REG_INT */
    reg_value = l1rpp_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_BIT_SYNC_OK_I_MSK) >> L1RPP_FEGE_REG_INT_BIT_SYNC_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_SYNC_OK_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_SYNC_OK_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_SYNC_OK_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_SYNC_OK_I_poll", value );

    /* (0x00000080 bits 0) field SYNC_OK_I of register PMC_L1RPP_FEGE_REG_INT */
    return l1rpp_fege_reg_INT_poll( b_ptr,
                                    h_ptr,
                                    L1RPP_FEGE_REG_INT_BIT_SYNC_OK_I_MSK,
                                    (value<<L1RPP_FEGE_REG_INT_BIT_SYNC_OK_I_OFF),
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
static INLINE void l1rpp_fege_field_LOS_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LOS_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LOS_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LOS_I_set_to_clear", value );

    /* (0x00000080 bits 15) field LOS_I of register PMC_L1RPP_FEGE_REG_INT */
    l1rpp_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_INT_BIT_LOS_I_MSK,
                                                  L1RPP_FEGE_REG_INT_BIT_LOS_I_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_LOS_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LOS_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 15) field LOS_I of register PMC_L1RPP_FEGE_REG_INT */
    reg_value = l1rpp_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_BIT_LOS_I_MSK) >> L1RPP_FEGE_REG_INT_BIT_LOS_I_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LOS_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LOS_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LOS_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_LOS_I_poll", value );

    /* (0x00000080 bits 15) field LOS_I of register PMC_L1RPP_FEGE_REG_INT */
    return l1rpp_fege_reg_INT_poll( b_ptr,
                                    h_ptr,
                                    L1RPP_FEGE_REG_INT_BIT_LOS_I_MSK,
                                    (value<<L1RPP_FEGE_REG_INT_BIT_LOS_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void l1rpp_fege_field_SDET_IN_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_SDET_IN_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_SDET_IN_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_SDET_IN_I_set_to_clear", value );

    /* (0x00000080 bits 14) field SDET_IN_I of register PMC_L1RPP_FEGE_REG_INT */
    l1rpp_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_INT_BIT_SDET_IN_I_MSK,
                                                  L1RPP_FEGE_REG_INT_BIT_SDET_IN_I_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_SDET_IN_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_SDET_IN_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 14) field SDET_IN_I of register PMC_L1RPP_FEGE_REG_INT */
    reg_value = l1rpp_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_BIT_SDET_IN_I_MSK) >> L1RPP_FEGE_REG_INT_BIT_SDET_IN_I_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_SDET_IN_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_SDET_IN_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_SDET_IN_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_SDET_IN_I_poll", value );

    /* (0x00000080 bits 14) field SDET_IN_I of register PMC_L1RPP_FEGE_REG_INT */
    return l1rpp_fege_reg_INT_poll( b_ptr,
                                    h_ptr,
                                    L1RPP_FEGE_REG_INT_BIT_SDET_IN_I_MSK,
                                    (value<<L1RPP_FEGE_REG_INT_BIT_SDET_IN_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void l1rpp_fege_field_ALIGN_CHANGE_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_ALIGN_CHANGE_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_ALIGN_CHANGE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_ALIGN_CHANGE_I_set_to_clear", value );

    /* (0x00000080 bits 13) field ALIGN_CHANGE_I of register PMC_L1RPP_FEGE_REG_INT */
    l1rpp_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_INT_BIT_ALIGN_CHANGE_I_MSK,
                                                  L1RPP_FEGE_REG_INT_BIT_ALIGN_CHANGE_I_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_ALIGN_CHANGE_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_ALIGN_CHANGE_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 13) field ALIGN_CHANGE_I of register PMC_L1RPP_FEGE_REG_INT */
    reg_value = l1rpp_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_BIT_ALIGN_CHANGE_I_MSK) >> L1RPP_FEGE_REG_INT_BIT_ALIGN_CHANGE_I_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_ALIGN_CHANGE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_ALIGN_CHANGE_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_ALIGN_CHANGE_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_ALIGN_CHANGE_I_poll", value );

    /* (0x00000080 bits 13) field ALIGN_CHANGE_I of register PMC_L1RPP_FEGE_REG_INT */
    return l1rpp_fege_reg_INT_poll( b_ptr,
                                    h_ptr,
                                    L1RPP_FEGE_REG_INT_BIT_ALIGN_CHANGE_I_MSK,
                                    (value<<L1RPP_FEGE_REG_INT_BIT_ALIGN_CHANGE_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void l1rpp_fege_field_RX_ERR_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_RX_ERR_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_RX_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_RX_ERR_I_set_to_clear", value );

    /* (0x00000080 bits 9) field RX_ERR_I of register PMC_L1RPP_FEGE_REG_INT */
    l1rpp_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_INT_BIT_RX_ERR_I_MSK,
                                                  L1RPP_FEGE_REG_INT_BIT_RX_ERR_I_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_RX_ERR_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_RX_ERR_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 9) field RX_ERR_I of register PMC_L1RPP_FEGE_REG_INT */
    reg_value = l1rpp_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_BIT_RX_ERR_I_MSK) >> L1RPP_FEGE_REG_INT_BIT_RX_ERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_RX_ERR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_RX_ERR_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_RX_ERR_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_RX_ERR_I_poll", value );

    /* (0x00000080 bits 9) field RX_ERR_I of register PMC_L1RPP_FEGE_REG_INT */
    return l1rpp_fege_reg_INT_poll( b_ptr,
                                    h_ptr,
                                    L1RPP_FEGE_REG_INT_BIT_RX_ERR_I_MSK,
                                    (value<<L1RPP_FEGE_REG_INT_BIT_RX_ERR_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void l1rpp_fege_field_LCV_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LCV_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LCV_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LCV_I_set_to_clear", value );

    /* (0x00000080 bits 8) field LCV_I of register PMC_L1RPP_FEGE_REG_INT */
    l1rpp_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_INT_BIT_LCV_I_MSK,
                                                  L1RPP_FEGE_REG_INT_BIT_LCV_I_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_LCV_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LCV_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 8) field LCV_I of register PMC_L1RPP_FEGE_REG_INT */
    reg_value = l1rpp_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_BIT_LCV_I_MSK) >> L1RPP_FEGE_REG_INT_BIT_LCV_I_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LCV_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LCV_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LCV_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_LCV_I_poll", value );

    /* (0x00000080 bits 8) field LCV_I of register PMC_L1RPP_FEGE_REG_INT */
    return l1rpp_fege_reg_INT_poll( b_ptr,
                                    h_ptr,
                                    L1RPP_FEGE_REG_INT_BIT_LCV_I_MSK,
                                    (value<<L1RPP_FEGE_REG_INT_BIT_LCV_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                                          enet_fege_handle_t *h_ptr,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_set_to_clear( l1rpp_fege_buffer_t *b_ptr,
                                                                          enet_fege_handle_t *h_ptr,
                                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_set_to_clear", value );

    /* (0x00000080 bits 7) field AUTO_NEG_LP_ABLTY_UPD_I of register PMC_L1RPP_FEGE_REG_INT */
    l1rpp_fege_reg_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  L1RPP_FEGE_REG_INT_BIT_AUTO_NEG_LP_ABLTY_UPD_I_MSK,
                                                  L1RPP_FEGE_REG_INT_BIT_AUTO_NEG_LP_ABLTY_UPD_I_OFF,
                                                  value);
}

static INLINE UINT32 l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 7) field AUTO_NEG_LP_ABLTY_UPD_I of register PMC_L1RPP_FEGE_REG_INT */
    reg_value = l1rpp_fege_reg_INT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_BIT_AUTO_NEG_LP_ABLTY_UPD_I_MSK) >> L1RPP_FEGE_REG_INT_BIT_AUTO_NEG_LP_ABLTY_UPD_I_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                                  enet_fege_handle_t *h_ptr,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                                  enet_fege_handle_t *h_ptr,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_poll", value );

    /* (0x00000080 bits 7) field AUTO_NEG_LP_ABLTY_UPD_I of register PMC_L1RPP_FEGE_REG_INT */
    return l1rpp_fege_reg_INT_poll( b_ptr,
                                    h_ptr,
                                    L1RPP_FEGE_REG_INT_BIT_AUTO_NEG_LP_ABLTY_UPD_I_MSK,
                                    (value<<L1RPP_FEGE_REG_INT_BIT_AUTO_NEG_LP_ABLTY_UPD_I_OFF),
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
static INLINE void l1rpp_fege_field_LOS_E_set( l1rpp_fege_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LOS_E_set( l1rpp_fege_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LOS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LOS_E_set", value );

    /* (0x00000084 bits 15) field LOS_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    l1rpp_fege_reg_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     L1RPP_FEGE_REG_INT_EN_BIT_LOS_E_MSK,
                                     L1RPP_FEGE_REG_INT_EN_BIT_LOS_E_OFF,
                                     value);
}

static INLINE UINT32 l1rpp_fege_field_LOS_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LOS_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 15) field LOS_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    reg_value = l1rpp_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_EN_BIT_LOS_E_MSK) >> L1RPP_FEGE_REG_INT_EN_BIT_LOS_E_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LOS_E_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_SDET_IN_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_SDET_IN_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_SDET_IN_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_SDET_IN_E_set", value );

    /* (0x00000084 bits 14) field SDET_IN_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    l1rpp_fege_reg_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     L1RPP_FEGE_REG_INT_EN_BIT_SDET_IN_E_MSK,
                                     L1RPP_FEGE_REG_INT_EN_BIT_SDET_IN_E_OFF,
                                     value);
}

static INLINE UINT32 l1rpp_fege_field_SDET_IN_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_SDET_IN_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 14) field SDET_IN_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    reg_value = l1rpp_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_EN_BIT_SDET_IN_E_MSK) >> L1RPP_FEGE_REG_INT_EN_BIT_SDET_IN_E_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_SDET_IN_E_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_ALIGN_CHANGE_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_ALIGN_CHANGE_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_ALIGN_CHANGE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_ALIGN_CHANGE_E_set", value );

    /* (0x00000084 bits 13) field ALIGN_CHANGE_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    l1rpp_fege_reg_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     L1RPP_FEGE_REG_INT_EN_BIT_ALIGN_CHANGE_E_MSK,
                                     L1RPP_FEGE_REG_INT_EN_BIT_ALIGN_CHANGE_E_OFF,
                                     value);
}

static INLINE UINT32 l1rpp_fege_field_ALIGN_CHANGE_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_ALIGN_CHANGE_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 13) field ALIGN_CHANGE_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    reg_value = l1rpp_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_EN_BIT_ALIGN_CHANGE_E_MSK) >> L1RPP_FEGE_REG_INT_EN_BIT_ALIGN_CHANGE_E_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_ALIGN_CHANGE_E_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_LINK_MON_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LINK_MON_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LINK_MON_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LINK_MON_E_set", value );

    /* (0x00000084 bits 12) field LINK_MON_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    l1rpp_fege_reg_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     L1RPP_FEGE_REG_INT_EN_BIT_LINK_MON_E_MSK,
                                     L1RPP_FEGE_REG_INT_EN_BIT_LINK_MON_E_OFF,
                                     value);
}

static INLINE UINT32 l1rpp_fege_field_LINK_MON_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LINK_MON_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 12) field LINK_MON_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    reg_value = l1rpp_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_EN_BIT_LINK_MON_E_MSK) >> L1RPP_FEGE_REG_INT_EN_BIT_LINK_MON_E_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LINK_MON_E_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_FEC_STAT_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_FEC_STAT_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_FEC_STAT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_FEC_STAT_E_set", value );

    /* (0x00000084 bits 11) field FEC_STAT_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    l1rpp_fege_reg_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     L1RPP_FEGE_REG_INT_EN_BIT_FEC_STAT_E_MSK,
                                     L1RPP_FEGE_REG_INT_EN_BIT_FEC_STAT_E_OFF,
                                     value);
}

static INLINE UINT32 l1rpp_fege_field_FEC_STAT_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_FEC_STAT_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 11) field FEC_STAT_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    reg_value = l1rpp_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_EN_BIT_FEC_STAT_E_MSK) >> L1RPP_FEGE_REG_INT_EN_BIT_FEC_STAT_E_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_FEC_STAT_E_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_CARRIER_STAT_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_CARRIER_STAT_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_CARRIER_STAT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_CARRIER_STAT_E_set", value );

    /* (0x00000084 bits 10) field CARRIER_STAT_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    l1rpp_fege_reg_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     L1RPP_FEGE_REG_INT_EN_BIT_CARRIER_STAT_E_MSK,
                                     L1RPP_FEGE_REG_INT_EN_BIT_CARRIER_STAT_E_OFF,
                                     value);
}

static INLINE UINT32 l1rpp_fege_field_CARRIER_STAT_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_CARRIER_STAT_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 10) field CARRIER_STAT_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    reg_value = l1rpp_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_EN_BIT_CARRIER_STAT_E_MSK) >> L1RPP_FEGE_REG_INT_EN_BIT_CARRIER_STAT_E_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_CARRIER_STAT_E_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_RX_ERR_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_RX_ERR_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_RX_ERR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_RX_ERR_E_set", value );

    /* (0x00000084 bits 9) field RX_ERR_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    l1rpp_fege_reg_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     L1RPP_FEGE_REG_INT_EN_BIT_RX_ERR_E_MSK,
                                     L1RPP_FEGE_REG_INT_EN_BIT_RX_ERR_E_OFF,
                                     value);
}

static INLINE UINT32 l1rpp_fege_field_RX_ERR_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_RX_ERR_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 9) field RX_ERR_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    reg_value = l1rpp_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_EN_BIT_RX_ERR_E_MSK) >> L1RPP_FEGE_REG_INT_EN_BIT_RX_ERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_RX_ERR_E_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_LCV_E_set( l1rpp_fege_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LCV_E_set( l1rpp_fege_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LCV_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LCV_E_set", value );

    /* (0x00000084 bits 8) field LCV_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    l1rpp_fege_reg_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     L1RPP_FEGE_REG_INT_EN_BIT_LCV_E_MSK,
                                     L1RPP_FEGE_REG_INT_EN_BIT_LCV_E_OFF,
                                     value);
}

static INLINE UINT32 l1rpp_fege_field_LCV_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LCV_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 8) field LCV_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    reg_value = l1rpp_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_EN_BIT_LCV_E_MSK) >> L1RPP_FEGE_REG_INT_EN_BIT_LCV_E_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LCV_E_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_E_set", value );

    /* (0x00000084 bits 7) field AUTO_NEG_LP_ABLTY_UPD_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    l1rpp_fege_reg_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     L1RPP_FEGE_REG_INT_EN_BIT_AUTO_NEG_LP_ABLTY_UPD_E_MSK,
                                     L1RPP_FEGE_REG_INT_EN_BIT_AUTO_NEG_LP_ABLTY_UPD_E_OFF,
                                     value);
}

static INLINE UINT32 l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 7) field AUTO_NEG_LP_ABLTY_UPD_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    reg_value = l1rpp_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_EN_BIT_AUTO_NEG_LP_ABLTY_UPD_E_MSK) >> L1RPP_FEGE_REG_INT_EN_BIT_AUTO_NEG_LP_ABLTY_UPD_E_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_E_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_LINK_OK_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_LINK_OK_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_LINK_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_LINK_OK_E_set", value );

    /* (0x00000084 bits 4) field LINK_OK_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    l1rpp_fege_reg_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     L1RPP_FEGE_REG_INT_EN_BIT_LINK_OK_E_MSK,
                                     L1RPP_FEGE_REG_INT_EN_BIT_LINK_OK_E_OFF,
                                     value);
}

static INLINE UINT32 l1rpp_fege_field_LINK_OK_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LINK_OK_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 4) field LINK_OK_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    reg_value = l1rpp_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_EN_BIT_LINK_OK_E_MSK) >> L1RPP_FEGE_REG_INT_EN_BIT_LINK_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LINK_OK_E_get", value );

    return value;
}
static INLINE void l1rpp_fege_field_SYNC_OK_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_field_SYNC_OK_E_set( l1rpp_fege_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "l1rpp_fege_field_SYNC_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "l1rpp_fege_field_SYNC_OK_E_set", value );

    /* (0x00000084 bits 0) field SYNC_OK_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    l1rpp_fege_reg_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     L1RPP_FEGE_REG_INT_EN_BIT_SYNC_OK_E_MSK,
                                     L1RPP_FEGE_REG_INT_EN_BIT_SYNC_OK_E_OFF,
                                     value);
}

static INLINE UINT32 l1rpp_fege_field_SYNC_OK_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_SYNC_OK_E_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 0) field SYNC_OK_E of register PMC_L1RPP_FEGE_REG_INT_EN */
    reg_value = l1rpp_fege_reg_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_EN_BIT_SYNC_OK_E_MSK) >> L1RPP_FEGE_REG_INT_EN_BIT_SYNC_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_SYNC_OK_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 l1rpp_fege_field_LOS_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LOS_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 15) field LOS_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    reg_value = l1rpp_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_STAT_BIT_LOS_V_MSK) >> L1RPP_FEGE_REG_INT_STAT_BIT_LOS_V_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LOS_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LOS_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LOS_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_LOS_V_poll", value );

    /* (0x00000088 bits 15) field LOS_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    return l1rpp_fege_reg_INT_STAT_poll( b_ptr,
                                         h_ptr,
                                         L1RPP_FEGE_REG_INT_STAT_BIT_LOS_V_MSK,
                                         (value<<L1RPP_FEGE_REG_INT_STAT_BIT_LOS_V_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE UINT32 l1rpp_fege_field_SDET_IN_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_SDET_IN_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 14) field SDET_IN_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    reg_value = l1rpp_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_STAT_BIT_SDET_IN_V_MSK) >> L1RPP_FEGE_REG_INT_STAT_BIT_SDET_IN_V_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_SDET_IN_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_SDET_IN_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_SDET_IN_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_SDET_IN_V_poll", value );

    /* (0x00000088 bits 14) field SDET_IN_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    return l1rpp_fege_reg_INT_STAT_poll( b_ptr,
                                         h_ptr,
                                         L1RPP_FEGE_REG_INT_STAT_BIT_SDET_IN_V_MSK,
                                         (value<<L1RPP_FEGE_REG_INT_STAT_BIT_SDET_IN_V_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE UINT32 l1rpp_fege_field_ALIGN_CHANGE_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_ALIGN_CHANGE_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 13) field ALIGN_CHANGE_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    reg_value = l1rpp_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_STAT_BIT_ALIGN_CHANGE_V_MSK) >> L1RPP_FEGE_REG_INT_STAT_BIT_ALIGN_CHANGE_V_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_ALIGN_CHANGE_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_ALIGN_CHANGE_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_ALIGN_CHANGE_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_ALIGN_CHANGE_V_poll", value );

    /* (0x00000088 bits 13) field ALIGN_CHANGE_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    return l1rpp_fege_reg_INT_STAT_poll( b_ptr,
                                         h_ptr,
                                         L1RPP_FEGE_REG_INT_STAT_BIT_ALIGN_CHANGE_V_MSK,
                                         (value<<L1RPP_FEGE_REG_INT_STAT_BIT_ALIGN_CHANGE_V_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE UINT32 l1rpp_fege_field_LINK_MON_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LINK_MON_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 12) field LINK_MON_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    reg_value = l1rpp_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_STAT_BIT_LINK_MON_V_MSK) >> L1RPP_FEGE_REG_INT_STAT_BIT_LINK_MON_V_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LINK_MON_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_MON_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_MON_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_LINK_MON_V_poll", value );

    /* (0x00000088 bits 12) field LINK_MON_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    return l1rpp_fege_reg_INT_STAT_poll( b_ptr,
                                         h_ptr,
                                         L1RPP_FEGE_REG_INT_STAT_BIT_LINK_MON_V_MSK,
                                         (value<<L1RPP_FEGE_REG_INT_STAT_BIT_LINK_MON_V_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE UINT32 l1rpp_fege_field_FEC_STAT_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_FEC_STAT_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 11) field FEC_STAT_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    reg_value = l1rpp_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_STAT_BIT_FEC_STAT_V_MSK) >> L1RPP_FEGE_REG_INT_STAT_BIT_FEC_STAT_V_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_FEC_STAT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_FEC_STAT_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_FEC_STAT_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_FEC_STAT_V_poll", value );

    /* (0x00000088 bits 11) field FEC_STAT_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    return l1rpp_fege_reg_INT_STAT_poll( b_ptr,
                                         h_ptr,
                                         L1RPP_FEGE_REG_INT_STAT_BIT_FEC_STAT_V_MSK,
                                         (value<<L1RPP_FEGE_REG_INT_STAT_BIT_FEC_STAT_V_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE UINT32 l1rpp_fege_field_CARRIER_STAT_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_CARRIER_STAT_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 10) field CARRIER_STAT_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    reg_value = l1rpp_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_STAT_BIT_CARRIER_STAT_V_MSK) >> L1RPP_FEGE_REG_INT_STAT_BIT_CARRIER_STAT_V_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_CARRIER_STAT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_CARRIER_STAT_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_CARRIER_STAT_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                         enet_fege_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_CARRIER_STAT_V_poll", value );

    /* (0x00000088 bits 10) field CARRIER_STAT_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    return l1rpp_fege_reg_INT_STAT_poll( b_ptr,
                                         h_ptr,
                                         L1RPP_FEGE_REG_INT_STAT_BIT_CARRIER_STAT_V_MSK,
                                         (value<<L1RPP_FEGE_REG_INT_STAT_BIT_CARRIER_STAT_V_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE UINT32 l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 7) field AUTO_NEG_LP_ABLTY_UPD_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    reg_value = l1rpp_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_STAT_BIT_AUTO_NEG_LP_ABLTY_UPD_V_MSK) >> L1RPP_FEGE_REG_INT_STAT_BIT_AUTO_NEG_LP_ABLTY_UPD_V_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                                  enet_fege_handle_t *h_ptr,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                                  enet_fege_handle_t *h_ptr,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_V_poll", value );

    /* (0x00000088 bits 7) field AUTO_NEG_LP_ABLTY_UPD_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    return l1rpp_fege_reg_INT_STAT_poll( b_ptr,
                                         h_ptr,
                                         L1RPP_FEGE_REG_INT_STAT_BIT_AUTO_NEG_LP_ABLTY_UPD_V_MSK,
                                         (value<<L1RPP_FEGE_REG_INT_STAT_BIT_AUTO_NEG_LP_ABLTY_UPD_V_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE UINT32 l1rpp_fege_field_LINK_OK_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LINK_OK_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 4) field LINK_OK_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    reg_value = l1rpp_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_STAT_BIT_LINK_OK_V_MSK) >> L1RPP_FEGE_REG_INT_STAT_BIT_LINK_OK_V_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LINK_OK_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_OK_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_OK_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_LINK_OK_V_poll", value );

    /* (0x00000088 bits 4) field LINK_OK_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    return l1rpp_fege_reg_INT_STAT_poll( b_ptr,
                                         h_ptr,
                                         L1RPP_FEGE_REG_INT_STAT_BIT_LINK_OK_V_MSK,
                                         (value<<L1RPP_FEGE_REG_INT_STAT_BIT_LINK_OK_V_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE UINT32 l1rpp_fege_field_SYNC_OK_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_SYNC_OK_V_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 0) field SYNC_OK_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    reg_value = l1rpp_fege_reg_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_INT_STAT_BIT_SYNC_OK_V_MSK) >> L1RPP_FEGE_REG_INT_STAT_BIT_SYNC_OK_V_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_SYNC_OK_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_SYNC_OK_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE l1rpp_fege_field_SYNC_OK_V_poll( l1rpp_fege_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "l1rpp_fege_field_SYNC_OK_V_poll", value );

    /* (0x00000088 bits 0) field SYNC_OK_V of register PMC_L1RPP_FEGE_REG_INT_STAT */
    return l1rpp_fege_reg_INT_STAT_poll( b_ptr,
                                         h_ptr,
                                         L1RPP_FEGE_REG_INT_STAT_BIT_SYNC_OK_V_MSK,
                                         (value<<L1RPP_FEGE_REG_INT_STAT_BIT_SYNC_OK_V_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset perf_cntr
 * ==================================================================================
 */
static INLINE void l1rpp_fege_lfield_PMON_CNTR_get( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void l1rpp_fege_lfield_PMON_CNTR_get( l1rpp_fege_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x000000cc bits 15:0) bits 0:15 use field PMON_CNTR of register PMC_L1RPP_FEGE_REG_PMON_CNT_0 */
    reg_value = l1rpp_fege_reg_PMON_CNT_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_PMON_CNT_0_BIT_PMON_CNTR_MSK) >> L1RPP_FEGE_REG_PMON_CNT_0_BIT_PMON_CNTR_OFF;
    value[0] |= field_value;

    /* (0x000000d0 bits 15:0) bits 16:31 use field PMON_CNTR of register PMC_L1RPP_FEGE_REG_PMON_CNT_1 */
    reg_value = l1rpp_fege_reg_PMON_CNT_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_PMON_CNT_1_BIT_PMON_CNTR_MSK) >> L1RPP_FEGE_REG_PMON_CNT_1_BIT_PMON_CNTR_OFF;
    value[0] |= (field_value << 16);

    /* (0x000000d4 bits 7:0) bits 32:39 use field PMON_CNTR of register PMC_L1RPP_FEGE_REG_PMON_CNT_2 */
    reg_value = l1rpp_fege_reg_PMON_CNT_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_PMON_CNT_2_BIT_PMON_CNTR_MSK) >> L1RPP_FEGE_REG_PMON_CNT_2_BIT_PMON_CNTR_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "l1rpp_fege_lfield_PMON_CNTR_get", value[1] , value[0] );

}
static INLINE UINT32 l1rpp_fege_lfield_range_PMON_CNTR_get( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_lfield_range_PMON_CNTR_get( l1rpp_fege_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_lfield_range_PMON_CNTR_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_lfield_range_PMON_CNTR_get", stop_bit, 39 );
    if (start_bit <= 15) {
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
    /* (0x000000cc bits 15:0) bits 0:15 use field PMON_CNTR of register PMC_L1RPP_FEGE_REG_PMON_CNT_0 */
        reg_value = l1rpp_fege_reg_PMON_CNT_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & L1RPP_FEGE_REG_PMON_CNT_0_BIT_PMON_CNTR_MSK)
                      >> L1RPP_FEGE_REG_PMON_CNT_0_BIT_PMON_CNTR_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_PMON_CNT_0_BIT_PMON_CNTR_MSK, L1RPP_FEGE_REG_PMON_CNT_0_BIT_PMON_CNTR_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 31) && (stop_bit >= 16)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 16) {
            subfield_offset = start_bit - 16;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 16 - start_bit;
            subfield_start_bit = 16;
        }
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000000d0 bits 15:0) bits 16:31 use field PMON_CNTR of register PMC_L1RPP_FEGE_REG_PMON_CNT_1 */
        reg_value = l1rpp_fege_reg_PMON_CNT_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & L1RPP_FEGE_REG_PMON_CNT_1_BIT_PMON_CNTR_MSK)
                      >> L1RPP_FEGE_REG_PMON_CNT_1_BIT_PMON_CNTR_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_PMON_CNT_1_BIT_PMON_CNTR_MSK, L1RPP_FEGE_REG_PMON_CNT_1_BIT_PMON_CNTR_OFF, field_value );
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
    /* (0x000000d4 bits 7:0) bits 32:39 use field PMON_CNTR of register PMC_L1RPP_FEGE_REG_PMON_CNT_2 */
        reg_value = l1rpp_fege_reg_PMON_CNT_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & L1RPP_FEGE_REG_PMON_CNT_2_BIT_PMON_CNTR_MSK)
                      >> L1RPP_FEGE_REG_PMON_CNT_2_BIT_PMON_CNTR_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_PMON_CNT_2_BIT_PMON_CNTR_MSK, L1RPP_FEGE_REG_PMON_CNT_2_BIT_PMON_CNTR_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_lfield_range_PMON_CNTR_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 l1rpp_fege_field_L1RPP_STATUS_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_L1RPP_STATUS_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 15) field L1RPP_STATUS of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    reg_value = l1rpp_fege_reg_CFG_AND_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_CFG_AND_STAT_BIT_L1RPP_STATUS_MSK) >> L1RPP_FEGE_REG_CFG_AND_STAT_BIT_L1RPP_STATUS_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_L1RPP_STATUS_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_SDET_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_SDET_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 13) field SDET of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    reg_value = l1rpp_fege_reg_CFG_AND_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_CFG_AND_STAT_BIT_SDET_MSK) >> L1RPP_FEGE_REG_CFG_AND_STAT_BIT_SDET_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_SDET_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_ENCDET_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_ENCDET_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 12) field ENCDET of register PMC_L1RPP_FEGE_REG_CFG_AND_STAT */
    reg_value = l1rpp_fege_reg_CFG_AND_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_CFG_AND_STAT_BIT_ENCDET_MSK) >> L1RPP_FEGE_REG_CFG_AND_STAT_BIT_ENCDET_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_ENCDET_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_R_PRBS_IND_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_R_PRBS_IND_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 15) field R_PRBS_IND of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
    reg_value = l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read( b_ptr,
                                                                    h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_R_PRBS_IND_MSK) >> L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_R_PRBS_IND_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_R_PRBS_IND_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_PRBS_IND_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_PRBS_IND_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 14) field PRBS_IND of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
    reg_value = l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read( b_ptr,
                                                                    h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PRBS_IND_MSK) >> L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PRBS_IND_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_PRBS_IND_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_R_PTRN_IND_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_R_PTRN_IND_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 13) field R_PTRN_IND of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
    reg_value = l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read( b_ptr,
                                                                    h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_R_PTRN_IND_MSK) >> L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_R_PTRN_IND_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_R_PTRN_IND_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_PTRN_IND_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_PTRN_IND_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 12) field PTRN_IND of register PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG */
    reg_value = l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read( b_ptr,
                                                                    h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_IND_MSK) >> L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_IND_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_PTRN_IND_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_AN_COMPLETE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_AN_COMPLETE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000009c bits 5) field AN_COMPLETE of register PMC_L1RPP_FEGE_REG_MII_STAT */
    reg_value = l1rpp_fege_reg_MII_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_STAT_BIT_AN_COMPLETE_MSK) >> L1RPP_FEGE_REG_MII_STAT_BIT_AN_COMPLETE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_AN_COMPLETE_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_REM_FLT_IND_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_REM_FLT_IND_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000009c bits 4) field REM_FLT_IND of register PMC_L1RPP_FEGE_REG_MII_STAT */
    reg_value = l1rpp_fege_reg_MII_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_STAT_BIT_REM_FLT_IND_MSK) >> L1RPP_FEGE_REG_MII_STAT_BIT_REM_FLT_IND_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_REM_FLT_IND_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_LNK_STATE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LNK_STATE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000009c bits 2) field LNK_STATE of register PMC_L1RPP_FEGE_REG_MII_STAT */
    reg_value = l1rpp_fege_reg_MII_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_STAT_BIT_LNK_STATE_MSK) >> L1RPP_FEGE_REG_MII_STAT_BIT_LNK_STATE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LNK_STATE_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_AN_LP_ABLTY_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_AN_LP_ABLTY_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 15:0) bits 0:15 use field AN_LP_ABLTY of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_read( b_ptr,
                                                                                         h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_BIT_AN_LP_ABLTY_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_BIT_AN_LP_ABLTY_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_AN_LP_ABLTY_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_range_AN_LP_ABLTY_get( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_AN_LP_ABLTY_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_AN_LP_ABLTY_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_AN_LP_ABLTY_get", stop_bit, 15 );
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
    /* (0x000000a4 bits 15:0) bits 0:15 use field AN_LP_ABLTY of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_read( b_ptr,
                                                                                         h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_BIT_AN_LP_ABLTY_MSK)
                  >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_BIT_AN_LP_ABLTY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_BIT_AN_LP_ABLTY_MSK, L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_BIT_AN_LP_ABLTY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_AN_LP_ABLTY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_RX_PAGE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_RX_PAGE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a8 bits 1) field RX_PAGE of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_EXPANSION_read( b_ptr,
                                                                    h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION_BIT_RX_PAGE_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION_BIT_RX_PAGE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_RX_PAGE_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_RESOLVE_PRIORITY_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_RESOLVE_PRIORITY_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a8 bits 0) field RESOLVE_PRIORITY of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_EXPANSION_read( b_ptr,
                                                                    h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION_BIT_RESOLVE_PRIORITY_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION_BIT_RESOLVE_PRIORITY_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_RESOLVE_PRIORITY_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_LOC_NP_TOGGLE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LOC_NP_TOGGLE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000ac bits 11) field LOC_NP_TOGGLE of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_read( b_ptr,
                                                                       h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_TOGGLE_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_TOGGLE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LOC_NP_TOGGLE_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_LNK_NP_NEXT_PAGE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LNK_NP_NEXT_PAGE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b0 bits 15) field LNK_NP_NEXT_PAGE of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_read( b_ptr,
                                                                                      h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_NEXT_PAGE_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_NEXT_PAGE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LNK_NP_NEXT_PAGE_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_LNK_NP_ACK_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LNK_NP_ACK_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b0 bits 14) field LNK_NP_ACK of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_read( b_ptr,
                                                                                      h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_ACK_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_ACK_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LNK_NP_ACK_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_LNK_NP_MSG_PAGE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LNK_NP_MSG_PAGE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b0 bits 13) field LNK_NP_MSG_PAGE of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_read( b_ptr,
                                                                                      h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_MSG_PAGE_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_MSG_PAGE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LNK_NP_MSG_PAGE_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_LNK_NP_ACK2_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LNK_NP_ACK2_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b0 bits 12) field LNK_NP_ACK2 of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_read( b_ptr,
                                                                                      h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_ACK2_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_ACK2_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LNK_NP_ACK2_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_LNK_NP_TOGGLE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LNK_NP_TOGGLE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b0 bits 11) field LNK_NP_TOGGLE of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_read( b_ptr,
                                                                                      h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_TOGGLE_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_TOGGLE_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LNK_NP_TOGGLE_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_LNK_NP_CODE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_LNK_NP_CODE_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b0 bits 10:0) bits 0:10 use field LNK_NP_CODE_FIELD of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_read( b_ptr,
                                                                                      h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_CODE_FIELD_MSK) >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_CODE_FIELD_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_LNK_NP_CODE_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_range_LNK_NP_CODE_get( l1rpp_fege_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_LNK_NP_CODE_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_LNK_NP_CODE_get", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_LNK_NP_CODE_get", stop_bit, 10 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 10) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 10;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000000b0 bits 10:0) bits 0:10 use field LNK_NP_CODE_FIELD of register PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE */
    reg_value = l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_read( b_ptr,
                                                                                      h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_CODE_FIELD_MSK)
                  >> L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_CODE_FIELD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_CODE_FIELD_MSK, L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_CODE_FIELD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_LNK_NP_CODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_BIT_ALIGN_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_BIT_ALIGN_get( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000dc bits 3:0) bits 0:3 use field BIT_ALIGN of register PMC_L1RPP_FEGE_REG_BIT_ALIGNMENT */
    reg_value = l1rpp_fege_reg_BIT_ALIGNMENT_read(  b_ptr, h_ptr);
    value = (reg_value & L1RPP_FEGE_REG_BIT_ALIGNMENT_BIT_BIT_ALIGN_MSK) >> L1RPP_FEGE_REG_BIT_ALIGNMENT_BIT_BIT_ALIGN_OFF;
    IOLOG( "%s -> 0x%08x", "l1rpp_fege_field_BIT_ALIGN_get", value );

    return value;
}
static INLINE UINT32 l1rpp_fege_field_range_BIT_ALIGN_get( l1rpp_fege_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 l1rpp_fege_field_range_BIT_ALIGN_get( l1rpp_fege_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "l1rpp_fege_field_range_BIT_ALIGN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "l1rpp_fege_field_range_BIT_ALIGN_get", stop_bit, 3 );
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
    /* (0x000000dc bits 3:0) bits 0:3 use field BIT_ALIGN of register PMC_L1RPP_FEGE_REG_BIT_ALIGNMENT */
    reg_value = l1rpp_fege_reg_BIT_ALIGNMENT_read(  b_ptr, h_ptr);
    field_value = (reg_value & L1RPP_FEGE_REG_BIT_ALIGNMENT_BIT_BIT_ALIGN_MSK)
                  >> L1RPP_FEGE_REG_BIT_ALIGNMENT_BIT_BIT_ALIGN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, L1RPP_FEGE_REG_BIT_ALIGNMENT_BIT_BIT_ALIGN_MSK, L1RPP_FEGE_REG_BIT_ALIGNMENT_BIT_BIT_ALIGN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "l1rpp_fege_field_range_BIT_ALIGN_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _L1RPP_FEGE_IO_INLINE_H */
